#pragma once
#include "cpoint.h"
#include "cenemy.h"
#include "define.h"

// Cấu trúc dữ liệu chứa thông tin ô mà trụ có thể tấn công (ô ảnh hưởng)
struct Treach
{
    cpoint pos;     // Tạo độ của ô đó
    int index;     // Thứ tự của ô trong danh sách các ô mà trụ có thể tấn công
    int step;      // Số bước mà đạn cần phải đi từ trụ đến ô đó

    Treach(const cpoint& p, int idx, int stp)
        : pos(p), index(idx), step(stp) {
    }
};

class ctower
{
private:
    cpoint _tpos;   // Tọa độ trụ
    vector<Treach> _treach;  // Mảng chứa các ô ảnh hưởng
public:
    ctower();
    ctower(cpoint tpos);

    cpoint getCurr() const;
    vector<Treach> getTreach() const;

    void setCurr(const cpoint& pos);

    // Hàm tạo ra danh sách các ô ảnh hưởng dựa trên vị trí các kẻ thù
    void createTreach(const vector<cenemy>& enemies);

    // Hàm tính toán hướng bắn của tháp dựa trên danh sách kẻ thù
    // Trả về hướng bắn và thứ tự của ô đó trên đường đi của kẻ thù (pathIndex)
    int calShootDirection(const vector<cenemy>& enemiesList, int& pathIndex) const;
};
