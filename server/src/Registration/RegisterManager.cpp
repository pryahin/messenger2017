#include <shared/include/crypto_pki.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Registration/RegisterManager.h"


using namespace m2::server;


RegisterManager::StringsPair RegisterManager::deserialize (const std::string &data) {
    pt::ptree request;
    StringsPair info;
    std::stringstream stream;
    stream << data;

    boost::property_tree::read_json (stream, request);
    info.serverString = request.get<std::string> ("server_string");
    info.clientString = request.get<std::string> ("client_string");

    return info;
}

HttpResponse::Code RegisterManager::doAction (const std::string &data, std::string &response) {
    StringsPair preparedData;
    try {
        preparedData = deserialize (data);
    }
    catch (const pt::ptree_error &e) {
        std::cout << e.what () << std::endl;
        response = createError ("client_string and decrypted server_string");
        return HttpResponse::Code::FORBIDEN;
    }
    userInfo info;
    auto result = createResponse (preparedData, info);

    if (result != response_result::ok) {
        return HttpResponse::Code::FORBIDEN;
    }

    db->CreateUser (info.fingerprint, info.clientPublicKey);

    return HttpResponse::Code::OK;
}


RegisterManager::RegisterManager (Database *db)
        : Manager (db) {

}

Manager::response_result RegisterManager::createResponse (const StringsPair &pair, userInfo &result) {

    std::string serverCheck;
    std::string clientString;
    std::string clientKey;
    try {
        auto serv_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider (db->getPublicServerKey (), true);
        auto server_string = serv_cryptor.decrypt (pair.serverString);
        auto uuid = boost::uuids::random_generator () ();
        auto stringLenTemp = boost::uuids::to_string (uuid);
        serverCheck = server_string.substr (0, stringLenTemp.size ());
        clientKey = server_string.substr (stringLenTemp.size ());
        auto cli_cryptor = m2::crypto::common::OpenSSL_RSA_CryptoProvider (clientKey, true);
        std::string client_string = cli_cryptor.encrypt (pair.clientString);

    }
        catch (const m2::crypto::common::CryptoError &e) {

            std::cout << e.what ();
        }

    if (serverCheck != clientString) {
        return Manager::response_result::wrong_response;
    }

    result.clientPublicKey = clientKey;
    m2::crypto::common::OpenSSL_RSA_CryptoProvider orca (clientKey, true);
//    result.fingerprint = orca.fingerprint ();

    return response_result::ok;
}
