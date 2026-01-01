#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <algorithm>
#include <windows.h>

void ClearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written, consoleSize;
    
    //获取控制台缓冲区信息
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

    //用空格填充整个缓冲区
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, {0, 0}, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, {0, 0}, &written);

    //将光标移动到左上角
    SetConsoleCursorPosition(hConsole, {0, 0});
}

class StudentMain {
private:
    std::string Name;
    std::string idNum;
    int Score;

public:
    StudentMain() : Name("Unknown"), idNum("Unknown"), Score(0) {} //默认构建参数
    StudentMain(const std::string& Name, const std::string& idNum, int Score) : Name(Name), idNum(idNum), Score(Score) {}

    //访问内部内容
    std::string GetName() const { return Name; }
    std::string GetidNum() const { return idNum; }
    int GetScore() const { return Score; }

    //更改内部内容
    void SetName(const std::string& NewName) { Name = NewName; }
    void SetidNum(const std::string& NewidNum) { idNum = NewidNum; }
    void SetScore(int NewScore) { Score = NewScore; }

    //输出内部内容
    void PrintInfo() const { std::cout << "姓名: " << std::setw(12) << Name << ", 学号: " << std::setw(16) << idNum << ", 分数: " << std::setw(4) << Score << std::endl; }
};

std::vector<StudentMain> SystemData;
int SortNow = -1;
int SortGo = 1;
int SignNow = -1;

void HelpInfo() {
    ClearConsole();
    std::cout << "----------可用指令----------\n";
    std::cout << "display: 显示所有学生数据\n";
    std::cout << "add: 数据添加模式\n";
    std::cout << "get: 数据查找模式\n";
    std::cout << "edit: 数据修改模式\n";
    std::cout << "remove: 数据删除模式\n";
    std::cout << "import: 数据导入\n";
    std::cout << "export: 数据导出\n";
    std::cout << "sort: 列表整理\n";
    std::cout << "clear: 清除所有数据\n";
    std::cout << "quit: 退出程序\n";
    std::cout << "--------共10条指令--------\n";
}

void DisplayAll() {
    ClearConsole();
    std::cout << "----------数据---------\n";
    for (const auto& infos : SystemData) infos.PrintInfo();
    std::cout << "------共计" << SystemData.size() << "人-------\n";
}

//添加学生数据，后期要根据排序修改
void AddStudent(const std::string& Name, const std::string& idNum, int Score) { 
    SystemData.emplace_back(Name, idNum, Score);
    SortNow = -1;
}

void AddMode() {
    ClearConsole();
    std::cout << "您已进入添加模式, 请依次输入学生姓名、学生学号、学生分数: ";
    std::string Name, idNum;
    int Score;
    while (std::cin >> Name >> idNum >> Score) {
        AddStudent(Name, idNum, Score);
        std::cout << "成功添加该数据到数据库中\n";
        ClearConsole();
        std::cout << "请问是否继续添加？ y or n? ";
        char c;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 'y') {
            std::cout << "请依次输入学生姓名、学生学号、学生分数: ";
            continue;
        }
        else {
            std::cout << "已退出添加模式\n";
            break;
        }
    }
}

//查找函数
int FindbyName(const std::string& Name) {
    int sizes = SystemData.size();
    for (int i = 0; i < sizes; ++i) {
        if (SystemData[i].GetName() == Name) return i;
    }
    return -1;
}

int FindbyidNum(const std::string& idNum) {
    int sizes = SystemData.size();
    for (int i = 0; i < sizes; ++i) {
        if (SystemData[i].GetidNum() == idNum) return i;
    }
    return -1;
}

void EditGo() {
    std::cout << "请输入想修改的值  1.姓名 2.学号 3.成绩 4.退出修改 ";
    int type;
    while (std::cin >> type) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (type == 1) {
            std::cout << "请输入需要修改的姓名：";
            std::string Name;
            std::cin >> Name;
            SystemData[SignNow].SetName(Name);
            std::cout << "修改成功\n";
        }
        else if (type == 2) {
            std::cout << "请输入需要修改的学号：";
            std::string idNum;
            std::cin >> idNum;
            SystemData[SignNow].SetidNum(idNum);
            std::cout << "修改成功\n";
        }
        else if (type == 3) {
            std::cout << "请输入需要修改的成绩：";
            int Score;
            std::cin >> Score;
            SystemData[SignNow].SetScore(Score);
            std::cout << "修改成功\n";
        }
        else {
            std::cout << "已退出修改模式\n";
            break;
        }
        ClearConsole();
        std::cout << "还想要继续修改吗? y or n?  ";
        char c;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 'y') {
            std::cout << "请输入想修改的值  1.姓名 2.学号 3.成绩 4.退出修改 ";
            continue;
        }
        else break;
    }
}

void RemoveGo() {
    SystemData.erase(SystemData.begin() + SignNow);
    std::cout << "已成功删除该学生数据\n";
    SignNow = -1;
}

void GetMode() {
    ClearConsole();
    std::cout << "您已进入查找模式, 请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出查找模式 ";
    int type;
    while (std::cin >> type) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (type == 1) {
            std::cout << "请输入姓名：";
            std::string Name;
            std::cin >> Name;
            SignNow = FindbyName(Name);
        }
        else if (type == 2) {
            std::cout << "请输入学号：";
            std::string idNum;
            std::cin >> idNum;
            SignNow = FindbyidNum(idNum);
        }
        else {
            std::cout << "已退出查找模式\n";
            break;
        }
        if (SignNow != -1) {
            std::cout << "已找到该学生数据\n";
            SystemData[SignNow].PrintInfo();
            std::cout << "是否需要进行修改、删除或继续查找？ 1.修改 2.删除 3.继续查找 4.退出查找模式  ";
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == 1) {
                EditGo();
                ClearConsole();
                std::cout << "还想继续查找吗？ y or n?  ";
                char g;
                std::cin >> g;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (g == 'y') {
                    std::cout << "请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出查找模式 ";
                    continue;
                }
                else break;
            }
            else if (choice == 2) {
                RemoveGo();
                ClearConsole();
                std::cout << "还想继续查找吗？ y or n?  ";
                char g;
                std::cin >> g;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (g == 'y') {
                    std::cout << "请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出查找模式 ";
                    continue;
                }
                else break;
            }
            else if (choice == 3) {
                ClearConsole();
                std::cout << "请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出查找模式 ";
                continue;
            }
            else {
                std::cout << "已退出查找模式\n";
                break;
            }
        }
        else {
            std::cout << "输入数据有误，或者不存在该学生数据\n";
            std::cout << "请重新查找： 1.按姓名查找 2.按学号查找 3.退出查找模式 ";
            continue;
        }
    }
}

void EditMode() {
    ClearConsole();
    std::cout << "您已进入修改模式, 你需要先查找, 请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出修改模式 ";
    int type;
    while (std::cin >> type) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (type == 1) {
            std::cout << "请输入姓名：";
            std::string Name;
            std::cin >> Name;
            SignNow = FindbyName(Name);
        }
        else if (type == 2) {
            std::cout << "请输入学号：";
            std::string idNum;
            std::cin >> idNum;
            SignNow = FindbyidNum(idNum);
        }
        else {
            std::cout << "已退出修改模式\n";
            break;
        }
        if (SignNow != -1) {
            std::cout << "已找到该学生数据\n";
            SystemData[SignNow].PrintInfo();
            std::cout << "确认要修改吗？ y or n?  ";
            char h;
            std::cin >> h;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (h == 'y') {
                EditGo();
                ClearConsole();
            }
            std::cout << "还想继续查找吗？ y or n?  ";
            char g;
            std::cin >> g;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (g == 'y') {
                std::cout << "请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出修改模式 ";
                continue;
            }
            else {
                std::cout << "已退出修改模式\n";
                break;
            }
        }
        else {
            std::cout << "输入数据有误，或者不存在该学生数据\n";
            std::cout << "请重新查找： 1.按姓名查找 2.按学号查找 3.退出修改模式 ";
            continue;
        }
    }
}

void RemoveMode() {
    ClearConsole();
    std::cout << "您已进入删除模式, 你需要先查找, 请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出删除模式 ";
    int type;
    while (std::cin >> type) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (type == 1) {
            std::cout << "请输入姓名：";
            std::string Name;
            std::cin >> Name;
            SignNow = FindbyName(Name);
        }
        else if (type == 2) {
            std::cout << "请输入学号：";
            std::string idNum;
            std::cin >> idNum;
            SignNow = FindbyidNum(idNum);
        }
        else {
            std::cout << "已退出删除模式\n";
            break;
        }
        if (SignNow != -1) {
            std::cout << "已找到该学生数据\n";
            SystemData[SignNow].PrintInfo();
            std::cout << "确认要删除吗？ y or n?  ";
            char h;
            std::cin >> h;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (h == 'y') {
                RemoveGo();
                ClearConsole();
            }
            std::cout << "还想继续查找吗？ y or n?  ";
            char g;
            std::cin >> g;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (g == 'y') {
                std::cout << "请输入查找方式： 1.按姓名查找 2.按学号查找 3.退出删除模式 ";
                continue;
            }
            else {
                std::cout << "已退出删除模式\n";
                break;
            }
        }
        else {
            std::cout << "输入数据有误，或者不存在该学生数据\n";
            std::cout << "请重新查找： 1.按姓名查找 2.按学号查找 3.退出删除模式 ";
            continue;
        }
    }
}

void ImportFile() {
    ClearConsole();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "请输入你想导入的文件名(txt格式)：";
    std::string FileName;
    std::getline(std::cin, FileName);
    FileName += ".txt";
    std::ifstream files(FileName, std::ios::out);
    if (!files.is_open()) std::cout << "无法保存为文件！\n";
    else {
        std::string Names, idNums;
        int Scores;
        while (files >> Names >> idNums >> Scores) AddStudent(Names, idNums, Scores);
        files.close();
        std::cout << "已从" << FileName << "成功导入\n";
    }
}
void ExportFile() {
    ClearConsole();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "请输入你想导出的文件名(txt格式)：";
    std::string FileName;
    std::getline(std::cin, FileName);
    if (!FileName.empty()) FileName += ".txt";
    else FileName = "datas.txt";
    std::ofstream files(FileName, std::ios::out);
    if (!files.is_open()) std::cout << "无法保存为文件！\n";
    else {
        for (const auto& data : SystemData) files << std::setw(12) << data.GetName() << std::setw(16) << data.GetidNum() << std::setw(4) << data.GetScore() << "\n";
        files.close();
        std::cout << "已成功保存为" << FileName << "\n";
    }
}

void SortMode() {
    std::cout << "已进入排序模式，目前排序为：";
    switch (SortNow) {
        case -1: {
            std::cout << "未排序\n";
            break;
        }
        case 1: {
            std::cout << "按姓名排序";
            break;
        }
        case 2: {
            std::cout << "按学号排序";
            break;
        }
        case 3: {
            std::cout << "按成绩排序";
            break;
        }
    }
    if (SortNow != -1) {
        if (SortGo == 1) std::cout << "(升序排序)\n";
        else std::cout << "(降序排序)\n";
    }
    std::cout << "你想按什么进行排序?  1.按姓名排序 2.按学号排序 3.按成绩排序 4.退出排序模式  ";
    int SortC;
    std::cin >> SortC;
    if (SortC == 1) {
        SortNow = 1;
        std::cout << "请选择排序选择  1.升序排序 2.降序排序  ";
        int c;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 1) {
            std::sort(SystemData.begin(), SystemData.end(), [](const StudentMain& DataA, const StudentMain& DataB) {
                return DataA.GetName() < DataB.GetName();
            });
            SortGo = 1;
        }
        else {
            std::sort(SystemData.begin(), SystemData.end(), [](const StudentMain& DataA, const StudentMain& DataB) {
                return DataA.GetName() > DataB.GetName();
            });
            SortGo = 2;
        }
    }
    else if (SortC == 2) {
        SortNow = 2;
        std::cout << "请选择排序选择  1.升序排序 2.降序排序  ";
        int c;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 1) {
            std::sort(SystemData.begin(), SystemData.end(), [](const StudentMain& DataA, const StudentMain& DataB) {
                return DataA.GetidNum() < DataB.GetidNum();
            });
            SortGo = 1;
        }
        else {
            std::sort(SystemData.begin(), SystemData.end(), [](const StudentMain& DataA, const StudentMain& DataB) {
                return DataA.GetidNum() > DataB.GetidNum();
            });
            SortGo = 2;
        }
    }
    else if (SortC == 3) {
        SortNow = 3;
        std::cout << "请选择排序选择  1.升序排序 2.降序排序  ";
        int c;
        std::cin >> c;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 1) {
            std::sort(SystemData.begin(), SystemData.end(), [](const StudentMain& DataA, const StudentMain& DataB) {
                return DataA.GetScore() < DataB.GetScore();
            });
            SortGo = 1;
        }
        else {
            std::sort(SystemData.begin(), SystemData.end(), [](const StudentMain& DataA, const StudentMain& DataB) {
                return DataA.GetScore() > DataB.GetScore();
            });
            SortGo = 2;
        }
    }
    else std::cout << "已退出排序模式\n";
    ClearConsole();
    std::cout << "排序成功！\n";
    DisplayAll();
} 

void ClearAll() { SystemData.clear(); }

void QuitCmd() { exit(0); }

int main() {
    std::ios::sync_with_stdio(false);
    system("chcp 65001>nul");
    std::cout << "---------欢迎来到简易版学生信息系统---------\n";
    std::cout << "tips: 输入help查看可用指令\n";
    std::unordered_map<std::string, std::function<void()>> CommandMap;
    CommandMap["help"] = HelpInfo;
    CommandMap["display"] = DisplayAll;
    CommandMap["add"] = AddMode;
    CommandMap["get"] = GetMode;
    CommandMap["edit"] = EditMode;
    CommandMap["remove"] = RemoveMode;
    CommandMap["import"] = ImportFile;
    CommandMap["export"] = ExportFile;
    CommandMap["sort"] = SortMode;
    CommandMap["clear"] = ClearAll;
    CommandMap["quit"] = QuitCmd;
    std::string cmd;
    while (std::cin >> cmd) {
        auto it = CommandMap.find(cmd);
        if (it != CommandMap.end()) it->second();
        else std::cout << "未知指令！\n";
    }
    return 0;
}