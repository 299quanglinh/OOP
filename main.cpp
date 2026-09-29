/**************
Mã sinh viên: YOUR_STUDENT_ID
Họ tên: YOUR_NAME
**************/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>

using namespace std;

class Employee {
protected:
    string id;
    string fullName;
    double baseSalary;
public:
    // Constructor mặc định
    Employee() : id("UNKNOWN"), fullName("Unnamed employee"), baseSalary(0) {}
    // Constructor có mã và họ tên
    Employee(const string& id, const string& fullName)
        : id(id), fullName(fullName), baseSalary(0) {
        if (id.empty() || fullName.empty())
            throw invalid_argument("Ma va ho ten khong duoc rong.");
    }
    // Constructor đầy đủ
    Employee(const string& id, const string& fullName, double baseSalary)
        : id(id), fullName(fullName), baseSalary(baseSalary) {
        if (id.empty() || fullName.empty() || baseSalary < 0)
            throw invalid_argument("Du lieu Employee khong hop le.");
    }
    string getId() const {
        return id;
    }
    string getFullName() const {
        return fullName;
    }
    double getBaseSalary() const {
        return baseSalary;
    }
    // Nạp chồng: tăng theo số tiền cố định
    void increaseSalary(double amount) {
        if (amount <= 0)
            throw invalid_argument("So tien tang phai duong.");
        baseSalary += amount;
    }
    // Nạp chồng: true = %, false = số tiền cố định
    void increaseSalary(double value, bool byPercentage) {
        if (value <= 0)
            throw invalid_argument("Gia tri tang phai duong.");

        if (byPercentage)
            baseSalary += baseSalary * value / 100;
        else
            baseSalary += value;
    }
    virtual double calculateMonthlyCost() const {
        return baseSalary;
    }
    virtual void displayInfo() const {
        cout << "Employee | ID: " << id
             << " | Name: " << fullName
             << " | Base salary: " << baseSalary << endl;
    }
    virtual ~Employee() {
        cout << "[Destructor] Employee " << id << endl;
    }
};
class SoftwareEngineer : public Employee {
private:
    string primaryLanguage;
    double technicalAllowance;
public:
    // Constructor theo đề: chưa nhập lương/phụ cấp
    SoftwareEngineer(const string& id, const string& fullName,
                     const string& primaryLanguage)
        : Employee(id, fullName),
          primaryLanguage(primaryLanguage),
          technicalAllowance(0) {
        if (primaryLanguage.empty())
            throw invalid_argument("Ngon ngu lap trinh khong duoc rong.");
    }
    // Constructor đầy đủ
    SoftwareEngineer(const string& id, const string& fullName,
                     double baseSalary, const string& primaryLanguage,
                     double technicalAllowance)
        : Employee(id, fullName, baseSalary),
          primaryLanguage(primaryLanguage),
          technicalAllowance(technicalAllowance) {
        if (primaryLanguage.empty() || technicalAllowance < 0)
            throw invalid_argument("Du lieu SoftwareEngineer khong hop le.");
    }
    double calculateMonthlyCost() const override {
        return baseSalary + technicalAllowance;
    }
    void displayInfo() const override {
        cout << "SoftwareEngineer | ID: " << id
             << " | Name: " << fullName
             << " | Base salary: " << baseSalary
             << " | Language: " << primaryLanguage
             << " | Allowance: " << technicalAllowance
             << " | Monthly cost: " << calculateMonthlyCost()
             << endl;
    }
    ~SoftwareEngineer() override {
        cout << "[Destructor] SoftwareEngineer " << id << endl;
    }
};
class ProjectTeam {
private:
    string projectCode;
    string projectName;
    Employee* leader;              // Không sở hữu đối tượng
    vector<Employee*> members;     // Không sở hữu đối tượng
public:
    ProjectTeam(const string& projectCode, const string& projectName)
        : projectCode(projectCode), projectName(projectName), leader(nullptr) {
        if (projectCode.empty() || projectName.empty())
            throw invalid_argument("Ma va ten du an khong duoc rong.");
    }
    ProjectTeam(const string& projectCode, const string& projectName,
                Employee& leader)
        : ProjectTeam(projectCode, projectName) {
        addMember(leader, true);
    }
    bool contains(const string& employeeId) const {
        for (Employee* employee : members) {
            if (employee->getId() == employeeId)
                return true;
        }
        return false;
    }
    bool addMember(Employee& employee) {
        if (contains(employee.getId())) {
            cout << "Khong the them: " << employee.getId()
                 << " da co trong nhom.\n";
            return false;
        }
        members.push_back(&employee);
        return true;
    }
    bool addMember(Employee& employee, bool makeLeader) {
        if (!contains(employee.getId())) {
            members.push_back(&employee);
        }
        if (makeLeader) {
            leader = &employee;
        }
        return true;
    }
    bool removeMember(const string& employeeId) {
        if (leader != nullptr && leader->getId() == employeeId) {
            cout << "Khong duoc xoa truong nhom. Hay doi truong nhom truoc.\n";
            return false;
        }
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i]->getId() == employeeId) {
                members.erase(members.begin() + i);
                return true;
            }
        }
        cout << "Khong tim thay nhan su can xoa.\n";
        return false;
    }
    void changeLeader(Employee& employee) {
        if (!contains(employee.getId())) {
            members.push_back(&employee);
        }
        leader = &employee;
        cout << employee.getFullName() << " da la truong nhom moi.\n";
    }
    double calculateTotalMonthlyCost() const {
        double total = 0;
        for (Employee* employee : members) {
            total += employee->calculateMonthlyCost();
        }
        return total;
    }
    void displayTeam() const {
        cout << "\n========== PROJECT TEAM ==========\n";
        cout << "Code: " << projectCode << endl;
        cout << "Name: " << projectName << endl;
        if (leader != nullptr)
            cout << "Leader: " << leader->getFullName() << endl;
        else
            cout << "Leader: Chua co\n";
        cout << "Members:\n";
        for (Employee* employee : members) {
            employee->displayInfo(); // Đa hình
        }
        cout << "Total monthly cost: "
             << calculateTotalMonthlyCost() << endl;
        cout << "==================================\n";
    }
    ~ProjectTeam() {
        cout << "[Destructor] ProjectTeam " << projectCode
             << " chi huy danh sach, khong huy Employee.\n";
    }
};
int main() {
    cout << fixed << setprecision(2);
    // 1. Tạo 2 Employee bằng 2 constructor khác nhau
    Employee emp1("E01", "Nguyen Van A");
    Employee emp2("E02", "Nguyen Van B", 8000);
    // 2. Tạo 2 SoftwareEngineer bằng 2 constructor khác nhau
    SoftwareEngineer dev1("SE01", "Nguyen Van C", "C++");
    SoftwareEngineer dev2("SE02", "Nguyen Van D",
                          15000, "Python", 3000);
    // 3. Tăng lương cố định
    emp2.increaseSalary(1000);
    // 4. Tăng lương theo phần trăm
    dev2.increaseSalary(10, true);
    // 5. Tạo nhóm chưa có trưởng nhóm
    ProjectTeam team1("P01", "Project Management System");
    // 6. Thêm nhân sự thường
    team1.addMember(emp1);
    // 7. Thêm kỹ sư và đặt làm trưởng nhóm
    team1.addMember(dev2, true);
    // 8. Thử thêm trùng
    team1.addMember(emp1);
    // 9, 10. Hiển thị đa hình và tính chi phí
    team1.displayTeam();
    // 11. Thử xóa trưởng nhóm
    team1.removeMember("SE02");
    // 12. Đổi trưởng nhóm rồi xóa trưởng nhóm cũ
    team1.changeLeader(emp1);
    team1.removeMember("SE02");
    cout << "\nSau khi doi truong nhom va xoa SE02:\n";
    team1.displayTeam();
    // 13, 14. Một nhân sự thuộc nhiều nhóm; hủy nhóm thứ hai
    {
        ProjectTeam team2("P02", "Website Project", dev1);
        team2.addMember(emp1); // emp1 thuộc cả team1 và team2
        team2.displayTeam();
    }
    // 15. Nhân sự vẫn tồn tại sau khi team2 bị hủy
    cout << "\nKiem tra dev1 van ton tai sau khi team2 bi huy:\n";
    dev1.displayInfo();
    return 0;
}