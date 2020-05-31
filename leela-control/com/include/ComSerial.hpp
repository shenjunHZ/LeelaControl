#include "IComSerial.hpp"
#include "configurations/DefineView.hpp"

namespace applications
{
    class UserApp;
}
namespace com
{
class ComSerial : public IComSerial
{
public:
    ComSerial(const std::string& serial,
        const configurations::comConfigParams& config,
        applications::UserApp& userApp);
    int openCom() override;
    int closeCom() override;
    int writeCom(const std::string& data) override;
    int readCom(std::string& data) override;

protected:
    int initCom();

private:
    const std::string m_ttyCom;
    const configurations::comConfigParams& m_comConfig;
    applications::UserApp& m_userApp;
    int m_fd;
};
} // namespace com