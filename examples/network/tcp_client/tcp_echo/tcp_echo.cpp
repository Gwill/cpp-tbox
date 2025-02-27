/**
 * 实现一个tcp的客户端
 * 它主动连接指定的服务端，在连接成功后，服务器发送什么，它就回什么
 * 当连接被断开后，它再次尝试连接
 *
 * 本示例主要用于演示TcpClient的使用方法
 */

#include <iostream>

#include <tbox/network/tcp_client.h>

#include <tbox/base/log.h>
#include <tbox/base/log_output.h>
#include <tbox/base/scope_exit.hpp>
#include <tbox/event/signal_event.h>

using namespace std;
using namespace tbox;
using namespace tbox::event;
using namespace tbox::network;

void PrintUsage(const char *prog)
{
    cout << "Usage: " << prog << " <ip:port|localpath>" << endl
         << "Exp  : " << prog << " 127.0.0.1:12345" << endl
         << "       " << prog << " /tmp/test.sock" << endl;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        PrintUsage(argv[0]);
        return 0;
    }

    LogOutput_Enable();
    SockAddr bind_addr = SockAddr::FromString(argv[1]);

    Loop *sp_loop = Loop::New();
    SetScopeExitAction([sp_loop] { delete sp_loop; });

    TcpClient client(sp_loop);
    client.initialize(bind_addr);
    client.bind(&client);   //!< 绑定自己，即实现 echo
    client.start();

    //! 注册ctrl+C停止信号
    SignalEvent *sp_stop_ev = sp_loop->newSignalEvent();
    SetScopeExitAction([sp_stop_ev] { delete sp_stop_ev; });
    sp_stop_ev->initialize(SIGINT, Event::Mode::kOneshot);
    //! 指定ctrl+C时要做的事务
    sp_stop_ev->setCallback(
        [sp_loop, &client] (int) {
            client.stop();
            sp_loop->exitLoop();    //! (3) 退出事件循环
        }
    );
    sp_stop_ev->enable();

    LogInfo("service runing ...");
    sp_loop->runLoop();
    LogInfo("service stoped");

    client.cleanup();

    return 0;
}
