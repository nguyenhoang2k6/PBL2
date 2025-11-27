#include <app/App.h>
#include <SDL3/SDL_main.h>
#include <iostream>

// Đây là điểm bắt đầu duy nhất của toàn bộ chương trình
int main(int argc, char* argv[]) {
    
    // 1. Tạo đối tượng App trên Stack
    App myApp;

    // 2. Gọi hàm Init() (đã bao gồm Init của MainMenuScreen)
    // Đây là "người gác cổng" an toàn của chúng ta.
    if (myApp.init() == false) {
        
        // Nếu Init() thất bại (ví dụ: không tìm thấy font, không tạo được window)
        std::cerr << "Khởi tạo Ứng dụng thất bại! Thoát." << std::endl;
        return -1; // Thoát chương trình với mã lỗi
    }

    // 3. Nếu Init() thành công, bắt đầu vòng lặp chính (main loop)
    myApp.run();

    // 4. Khi vòng lặp 'run()' kết thúc (người dùng thoát)
    // Hàm main() kết thúc.
    // Hàm hủy ~App() sẽ tự động được gọi, dọn dẹp mọi thứ.
    return 0; // Thoát chương trình thành công
}