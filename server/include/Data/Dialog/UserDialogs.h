#ifndef M2_SERVER_USERDIALOGS_H
#define M2_SERVER_USERDIALOGS_H


#include "Data/User/User.h"
#include "Dialog.h"


namespace m2     {
namespace data   {
namespace dialog {

    using namespace user;


    /**
     *
     */
    class AUserDialogs
        : public TDialogSystem<ADialog>
    {
    public:

        AUserDialogs(const std::string&  Root, uuids::uuid Uid);

    public:

        /**
         * @param Uid - uid of chat (usually uid of contact)
         * @return - pointer on a Dialog
         */
        std::shared_ptr<ADialog>
        Get(const uuids::uuid& Uid)       override;

        /**
         * @return - pointer on a Dialog or nullptr
         */
        const
        std::shared_ptr<ADialog>
        Get(const uuids::uuid& Uid) const override;

    protected:

        MPtr create_dialog(uuids::uuid Uid);
    };


}}}

#endif //M2_SERVER_USERDIALOGS_H
