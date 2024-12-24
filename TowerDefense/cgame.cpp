#include "cgame.h"
#include "ctool.h"

atomic<bool> gameOver(false);

// Khai báo 1 biến bool toàn cục để ghi nhận khi nào chương trình kết thúc
// Có 2 loại kết thúc là: 
// 1. Toàn bộ enemy bị bắn chết
// 2. Có 1 enemy đi đến cuối đường

// Khởi tạo các biến mutex để quản lý việc thay đổi dữ liệu và in

mutex printMtx;
mutex enemyMtx; // Mutex for enemies list access


void cgame::addMap(const cmap& map)
{
    _map.push_back(map);
}

// Bắt đầu game bằng cách thêm 4 map vào list
void cgame::startGame()
{
    system("cls");

    for (int i = 1; i <= 4; ++i) {
        addMap(cmap(i));
    }
}

// Hàm chạy game (chạy các luồng enemy và đạn)
void cgame::processGame()
{
    if (_map.empty()) {
        return;
    }

    int option = 0;  // Mặc định chọn bản đồ đầu tiên
    int choice = 0;  // Lựa chọn bản đồ cuối cùng (để sử dụng sau)
    ctool::drawMapLevelText();
    while (true) {
        //system("cls");  // Xóa màn hình
        system("color E3");  // Đặt màu nền và màu chữ


        // Vẽ các ô cho từng bản đồ (1-4)
        ctool::printRectangle(40, 10, 20, 3);  // Ô cho Map 1
        ctool::printRectangle(40, 14, 20, 3);  // Ô cho Map 2
        ctool::printRectangle(40, 18, 20, 3); // Ô cho Map 3
        ctool::printRectangle(40, 22, 20, 3); // Ô cho Map 4

        // In các lựa chọn vào các ô chữ nhật
        for (int i = 0; i < 4; ++i) {
            // Nếu đang chọn mục này, tô màu xanh
            if (i == option) {
                setColor(12, 3);  // Highlight in blue
            }
            else {
                setColor(14, 3);  // Các mục còn lại dùng màu mặc định
            }

            // In các lựa chọn bản đồ vào giữa các ô chữ nhật
            ctool::GotoXY(47, 11 + i * 4);  // Vị trí chữ trong ô
            std::cout << "Map " << (i + 1);  // In "Map 1", "Map 2", ...
        }

        setColor(12, 3);  // Reset lại màu sắc

        // Nhận đầu vào từ người dùng (phím mũi tên hoặc Enter)
        char ch = _getch();

        // Điều hướng lên/xuống
        if (ch == KEY_UP) {  // Di chuyển lên
            option = (option > 0) ? option - 1 : 3;  // Nếu ở đầu, quay lại cuối
        }
        else if (ch == KEY_DOWN) {  // Di chuyển xuống
            option = (option + 1) % 4;  // Nếu ở cuối, quay lại đầu
        }
        else if (ch == KEY_ENTER) {  // Chọn
            choice = option + 1;  // Chọn bản đồ (Map 1, Map 2, ...)
            break;  // Thoát vòng lặp sau khi chọn
        }
        else if (ch == 27) {  // Phím ESC để quay lại
            return;  // Quay lại nếu nhấn ESC
        }
    }

    // Sau khi chọn xong, bạn có thể sử dụng lựa chọn cho bản đồ
    system("cls");
    system("color E3");  // Đặt màu nền và màu chữ
    setColor(14, 0);
    int mapIndex = choice - 1;  // Xác định chỉ mục bản đồ

    // Vẽ và hiển thị bản đồ đã chọn
    _map[mapIndex].drawMap();

    auto& map = _map[mapIndex];  // Lấy bản đồ đã chọn
    vector<cenemy>& enemies = map.getEnemies();

    int indexEnemy = 0;

    // Khởi tạo các luồng để di chuyển kẻ địch
    vector<thread> enemyThreads;
    for (auto& enemy : enemies) {
        enemyThreads.emplace_back(&cgame::enemyMovement, this, ref(enemy), mapIndex, indexEnemy);
        indexEnemy++;
    }

    endGame(enemyThreads);
}

// Hàm xử lý sau khi trò chơi kết thúc (có thể hởi người chơi về việc chơi lại hoặc ....)
void cgame::endGame(vector<thread>& enemyThreads)
{
    for (auto& t : enemyThreads)
    {
        if (t.joinable()) {
            t.join();
        }
    }

    if (gameOver.load())
    {

        system("cls");

        ctool::drawEndGameText();


        while (true)
        {
            if (_kbhit()) {
                char key = _getch();
                if (key == 27)
                {
                    system("cls");
                    exit(0);
                }
                else if (key == 13) {
                    _ISEXIT = false;
                    break;
                }
            }
        }
    }
}

// Hàm di chuyển và vẽ 1 enemy
void cgame::enemyMovement(cenemy& enemy, int mapIndex, int indexEnemy) {
    vector<cmap>& mapList = getMap();
    cmap& map = mapList[mapIndex];

    auto& enemies = map.getEnemies();
    auto copyEnemies = enemies;
    int numberEnemies = enemies.size();
    vector<cpoint> path = enemy.getPath();
    cpoint ENEMY = enemy.getCurr();
    double enemyHealth = enemy.getHealth();
    int enemySpeed = enemy.getSpeed();

    int enemyIndex = 0;

    auto& towers = map.getTowers();

    int step = 5;   // Khoảng cách giữa các enemy
    int timeDelay = indexEnemy * step * 500;    // Thời gian delay

    this_thread::sleep_for(chrono::milliseconds(timeDelay));    // Cho delay

    vector<thread> bullet_threads;  // Danh sách các luồng đạn
    vector<bool> bulletThreadStatus;    // Danh sách trạng thái của các luồng tương ứng (đã kết thúc chưa, chưa thì là False)

    int damage = 30;
    int threadIndex = 0; // Chỉ số để biểu thị thứ tự của luồng đạn

    string tmp;

    while (enemyIndex < path.size() && !gameOver.load() && enemy.isAlive())
    {
        {
            lock_guard<mutex> lock(printMtx);

            if (enemy.getHealth() > 0.7 * enemyHealth)
                cout << TEXT_GREEN_BG_LIGHT_YELLOW;     // Enemy còn nhiều máu thì vẽ màu xanh
            else if (enemy.getHealth() >= 0.5 * enemyHealth)
                cout << TEXT_YELLOW_BG_LIGHT_YELLOW;    // Enemy còn nửa máu thì vẽ màu vàng
            else if (enemy.getHealth() >= 0.2 * enemyHealth)
                cout << TEXT_RED_BG_LIGHT_YELLOW;       // Enemy thấp máu thì vẽ màu đỏ
            else
                cout << TEXT_BLACK_BG_LIGHT_YELLOW;     // Enemy gần cạn máu thì vẽ màu đen

            string tmp = "E";
            ctool::Draw(tmp, enemyIndex, path, ENEMY);
        }

        enemy.setCurr(path[enemyIndex]);
        enemy.setIndex(enemyIndex++);

        ENEMY = enemy.getCurr();

        {
            lock_guard<mutex> lock(enemyMtx);
            copyEnemies[indexEnemy] = enemy;

            for (auto& tower : towers)
            {
                auto path = map.createBulletPath(tower, copyEnemies);   // Tạo đường đi cho đạn

                if (path.empty())
                {
                    continue;
                }

                // Nếu đường đi tồn tại thì tạo đạn và nhét vào danh sách đạn trong map
                cbullet newBullet(path[0], damage, path);

                map.addBullet(newBullet);

                bulletThreadStatus.push_back(false);    // Thêm trạng thái false vào danh sách vì thread đạn mới tạo nên thread chưa kết thúc
                // Tạo thread đạn và nhét vào danh sách
                bullet_threads.emplace_back(thread(&cgame::bulletMovement, this, ref(newBullet), newBullet.getPath(), mapIndex, ref(bulletThreadStatus), threadIndex, enemySpeed));
                threadIndex++;
            }
        }

        // Tính thời gian delay để in
        int delayTime = floor(650 / sqrt(enemySpeed));

        this_thread::sleep_for(chrono::milliseconds(delayTime));
        {
            lock_guard<mutex> lock(printMtx);
            tmp = " ";
            ctool::Draw(tmp, enemyIndex - 1, path, ENEMY);
        }

        // Kiểm tra trong danh sách các thread đạn xem thread nào đã hoàn thành
        // Khi thread đạn hoàn thành có nghĩa là enemy đã bị bắn dính
        for (int i = 0; i < bullet_threads.size(); i++)
        {
            if (bulletThreadStatus[i] == true)
            {
                bulletThreadStatus[i] = false;      // Set về false để bỏ qua thread này vào lần tới

                enemy.decreaseHealth(damage);       // Trừ máu kẻ thù

                // TH: Enemy hết máu thì set biến alive về false
                if (enemy.getHealth() <= 0)
                {
                    enemy.setAlive(false);
                    {
                        lock_guard<mutex> lock(printMtx);
                        cout << TEXT_CYAN_BG_LIGHT_YELLOW;
                        tmp = "x";      // Hiệu úng khi enemy chết
                        ctool::Draw(tmp, enemyIndex, path, ENEMY);
                        this_thread::sleep_for(chrono::milliseconds(delayTime));
                        ctool::Draw(" ", enemyIndex, path, ENEMY);      // Xóa đi hiệu ứng
                    }

                    if (indexEnemy == numberEnemies - 1)
                    {
                        gameOver.store(true);       // Nếu enemy chết hết rồi thì end game
                        goto exit;
                    }

                    break;
                }
            }
        }

    }

exit:
    {
        lock_guard<mutex> lock(printMtx);
        cout << TEXT_CYAN_BG_LIGHT_YELLOW;
        tmp = " ";
        ctool::Draw(tmp, enemyIndex - 1, path, ENEMY);
    }

    for (auto& t : bullet_threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    if (enemyIndex == path.size())
        gameOver.store(true);   // Nếu enemy đi đến cuối đường rồi thì end game
}

// Hàm di chuyển và vẽ đạn
void cgame::bulletMovement(cbullet& bullet, vector<cpoint> path, int mapIndex, vector<bool>& bulletThreadStatus, int threadIndex, int enemySpeed)
{
    vector<cmap> mapList = getMap();
    cmap& map = mapList[mapIndex];

    cpoint BULLET = bullet.getCurr();
    int damage = bullet.getDamage();
    int bulletIndex = 0;

    string tmp;
    if (bullet.isActive())
    {
        while (bulletIndex < path.size() - 1 && !gameOver.load())
        {
            {
                std::lock_guard<std::mutex> lock(printMtx);
                cout << TEXT_PURPLE_BG_LIGHT_YELLOW;
                string tmp = "o";
                ctool::Draw(tmp, bulletIndex, path, BULLET);
            }

            bullet.setCurr(path[bulletIndex++]);

            BULLET = bullet.getCurr();

            int timeDelay = 700 / sqrt(enemySpeed);

            this_thread::sleep_for(chrono::milliseconds(timeDelay));

            {
                lock_guard<std::mutex> lock(printMtx);
                cpoint bulletPos = cpoint::fromXYToRowCol(BULLET);

                if (bulletPos.getC() == 1)
                    tmp = "+";
                else
                    tmp = " ";

                ctool::Draw(tmp, bulletIndex - 1, path, BULLET);
            }

            if (bulletIndex == path.size() - 2)
            {
                bulletThreadStatus[threadIndex] = true;
            }
        }
    }

    bullet.setActive(false);
}
