#include <calf/platform/windows/win32.hpp>
#include <calf/platform/windows/file_io.hpp>
#include <calf/platform/windows/system_services.hpp>
#include <calf/worker_service.hpp>

#include <functional>
#include <sstream>
#include <iostream>

int main(int argc, char* argv[]) {
  std::wstring pipe_name(L"\\\\.\\pipe\\ipc_sample");
  calf::pipe_message_service::message_received_handler handler = [](std::unique_ptr<calf::pipe_message>& message) {
    std::cout << "id=" << message->head()->id << " size=" << message->head()->size << std::endl;
  };
  calf::pipe_message_service service(pipe_name, calf::io_mode::open, handler);

  for (int i=0; i< 10; ++i) {
    auto message = std::make_unique<calf::pipe_message>(1 + i, 10);
    memset(message->data(), 'a' + i, 10);
    service.send_message(std::move(message));
  }

  std::thread thread(&calf::pipe_message_service::run, &service);
  thread.join();
  return 0;    
}