
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>
using namespace std;


int get_value(string section, string key, string fname, string &out_buf)
{
    fstream f;
    f.open(fname, ios_base::in);

    if(!f) return -2;

    string line;
    while(getline(f, line)){
        if(line.find(section) != string::npos)
            break;
    }
    
    if(f.eof()){
        f.close();
        return -1;
    }

    size_t x;
    while(getline(f, line)){
        if(line[0] == '[') break;
        if((x = line.find(key)) != string::npos){
            out_buf = line.substr(x + key.size() + 3, line.size() - (x + key.size() + 3));
            f.close();
            return 0;
        }
    }

    if(f.eof() || line[0] == '['){
        f.close();
        return 1;
    }

    return 0;
}

int set_value(string section, string key, string fname, string value = "", bool rem_key = 0)
{
    fstream f;
    f.open(fname, ios_base::in);

    ofstream f_out;
    f_out.open("./temp");

    string line;
    while(getline(f, line)){
        f_out << line << endl;        
        if(line.find("[" + section+ "]") != string::npos)
            break;
    }
    
    if(f.eof()){
        while(getline(f, line))
            f_out << line << endl;
        
        rename("./temp", fname.c_str());
        f.close();
        f_out.close();
        return -1;
    }

    size_t x;

    while(getline(f, line)){
        if(line[0] == '[') {
            f_out << line << endl;
            break;
        }
        if((x = line.find(key)) != string::npos){
            if(rem_key) break;
            line.replace(x + key.size() + 3, line.size() - (x + key.size() + 3), value);
            f_out << line << endl;
            break;
        }
        f_out << line << endl;
    }

    if(line[0] == '[' || f.eof()){
        while(getline(f, line))
            f_out << line << endl;
        rename("./temp", fname.c_str());

        f.close();
        f_out.close();
        return 1;
    }

    while(getline(f, line))
        f_out << line << endl;
    rename("./temp", fname.c_str());

    f.close();
    f_out.close();
    return 0;
}

int add_key(string section, string key, string fname, string value){
    fstream f;
    f.open(fname);

    ofstream f_out;
    f_out.open("./temp");

    string line;
    while(getline(f, line)){
        f_out << line << endl;
        if(line.find("[" + section + "]") != string::npos)
            break;
    }
    //cout << line;
    if(f.eof()){
        rename("./temp", fname.c_str());
        f.close();
        f_out.close();
        return -1;
    }
    
    int x;
    while((x = f.peek()) != '[' && x != EOF){
        //cout << x << endl;
        getline(f, line);
        //cout << line;
        f_out << line << endl;
        if(line.find("> " + key) != string::npos){
            while(getline(f, line))
                f_out << line << endl;

            rename("./temp", fname.c_str());
            f_out.close();
            f.close();
            return 1;
        }
    }

    f_out << "> " << key << " : " << value << endl;
    while(getline(f, line))
        f_out << line << endl;

    rename("./temp", fname.c_str());
    f_out.close();
    f.close();

    return 0;
}

bool add_section(string section, string fname)
{
    fstream f;
    f.open(fname);

    ofstream f_out;
    f_out.open("./temp");

    bool ins = true;
    string line;
    while(getline(f, line)){
        f_out << line << endl;
        if(line.find("[" + section + "]") != string::npos)
            ins = false;
    }
    if(ins) f_out << '[' << section << ']' << endl;

    rename("./temp", fname.c_str());
    f.close();
    f_out.close();
    return ins;
}

bool rem_section(string section, string fname)
{
    fstream f;
    f.open(fname, ios_base::in);

    ofstream f_out;
    f_out.open("./temp");

    string line;
    bool copy = true;
    bool out;
    while(getline(f, line)){
        if(line[0] == '[') copy = true;
        if(line.find("[" + section + "]") != string::npos){
            copy = false;
            out = false;
        }
        if(copy) f_out << line << endl;
    }

    rename("./temp", fname.c_str());
    f.close();
    f_out.close();

    return out;
}

bool dump_file(string fname)
{
    fstream f;
    f.open(fname);

    if(!f) return false;
    char c;
    f.get(c);
    if(c == '@'){
        f.unget();
        f.close();
        return false;
    }

    f.unget();

    string line;
    while(getline(f, line))
        cout << line << endl;
    
    return true;
}

bool dump_keys(vector <string> &out, string section, string fname)
{
    fstream f;
    f.open(fname);

    string line;
    while(getline(f, line))
        if(line.find(section) != string::npos)
            break;
    
    if(f.eof()){
        f.close();
        return false;
    }
    
    while(getline(f, line) && line[0] == '>')
        out.push_back(line.substr(1, line.find(" :") - 1));
    
    f.close();
    return true;
}

bool dump_section(string fname, string section)
{
    fstream f;
    f.open(fname);

    if(!f) return false;
    char c;
    f.get(c);
    if(c == '@'){
        f.unget();
        f.close();
        return false;
    }

    f.unget();

    string line;
    while(getline(f, line))
        if(line.find("[" + section + "]") != string::npos)
            break;
    if(f.eof()){
        f.close();
        return false;
    }
    cout << line << endl;
    while(getline(f, line)){
        if(line[0] == '[') break;
        cout << line << endl;
    }
    return true;
}

int get_key(string section, string &key, string fname, string value)
{
    fstream f;
    f.open(fname, ios_base::in);

    if(!f) return -2;

    string line;
    while(getline(f, line)){
        if(line.find(section) != string::npos)
            break;
    }
    
    if(f.eof()){
        f.close();
        return -1;
    }

    size_t x;
    while(getline(f, line)){
        if(line[0] == '[') break;
        if((x = line.find(value)) != string::npos){
            key = line.substr(2, x - 5);
            f.close();
            return 0;
        }
    }

    if(f.eof() || line[0] == '['){
        f.close();
        return 1;
    }

    return 0;
}

/*
All users:
    -> login
    -> logout
Admin : 
    -> Add a course to the course pool
    -> Delete a course from the course pool

    -> Add a student to the roll
    -> Remove a student from the roll

    -> Add a faculty member
    -> Remove a faculty member

    -> View student roll
    -> View course pool
    -> View faculty list
Faculty :
    -> Grade students for their courses
    -> View all students for courses
Student :
    -> View all courses and their grades
    -> Register and deregister for courses
    -> View course pool
*/

/*
Attached directory:

aims_directory:
    Students: Each file contains data and courses of one student.
        
    Courses: Each file contains the data and attending students of one course.
        
    Faculty: Each file contains faculty data and courses taken.
        
*/

const string aims = "./aims_directory/";

class User
{
    public:
    string name;
    string id;
    string email;

    
    void set_password(){
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter old password." << endl;
        string old;
        cin >> old;

        string pass;
        get_value("Passwords", id, aims + "pass.txt", pass);

        if(pass != old){
            cout << "Wrong." << endl;
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter new password." << endl;
        string new_p;
        cin >> new_p;

        set_value("Passwords", id, aims + "pass.txt", new_p);
        cout << "Changed." << endl;
    }
    void view_cmd(int stat){
        dump_section(aims + "commands", to_string(stat));
        if(stat != -1)
            dump_section(aims + "commands", "-1");
    }

    virtual void multiplexer(string command)
    {
        cout << "No poly" << endl;
    }
};

class Student : public User
{
    int current_sem;
    void reg_course();//-1 if no such course exists.
    void dereg_course();
    void view_all_courses();
    void view_grade(); //-1 if no such course registered

    public:
    Student(string &in_id){
        this->id = in_id;
        string dummy;
        get_value("Data", "Name", aims + "Students/" + in_id, this->name);
        get_value("Data", "Email ID", aims + "Students/" + in_id, this->email);
        get_value("Data", "Current semester", aims + "Students/" + in_id, dummy);
        this->current_sem = stoi(dummy);
    }

    void multiplexer(string line)
    {
            if(line == "rg"){
                reg_course();
                return;
            }

            if(line == "drg"){
                dereg_course();
                return;
            }

            if(line == "vc"){
                view_all_courses();
                return;
            }

            if(line == "vg"){
                view_grade();
                return;
            }
    }
    
};

class Faculty : public User
{
    void view_students_in_course(); //prints all students registered to the given course
    void view_courses(); //prints all owned courses
    void assign_grade(); //grades a given student for the given course.

    public:
    Faculty(string &in_id)
    {
        this->id = in_id;
        get_value("Data", "Name", aims + "Faculty/" + in_id, this->name);
        get_value("Data", "Email ID", aims + "Faculty/" + in_id, this->email);
    }

    void multiplexer(string line)
    {
            if(line == "vc"){
                view_courses();
                return;
            }

            if(line == "vsc"){
                view_students_in_course();
                return;
            }

            if(line == "ag"){
                assign_grade();
                return;
            }
    }
};

class Admin : public User
{
    void create_student(); //prints error if student already exists
    void remove_student(); //prints error if no such student exists
    void remove_student_from_course(); //accepts student and course id as input
    void add_student_to_course();
    void view_students();

    void create_course(); //prints error if course already exists
    void remove_course(); //prints error if no such course exists
    void view_courses();

    void create_faculty(); //prints error if faculty already exists
    void remove_faculty(); //prints error if no such faculty exists
    void add_faculty_courses();
    void remove_faculty_courses();
    void view_faculty();

    public:
    Admin()
    {
        this->id = "0";
        get_value("Data", "Name", aims + "admin", this->name);
        get_value("Data", "Email", aims + "admin", this->email);
    }


    void multiplexer(string line)
    {
        
            if(line == "crs"){
                create_student();
                return;
            }
            if(line == "crf"){
                create_faculty();
                return;
            }
            if(line == "vc"){
                view_courses();
                return;
            }
            if(line == "vs"){
                view_students();
                return;
            }
            if(line == "vf"){
                view_faculty();
                return;
            }
            if(line == "crc"){
                create_course();
                return;
            }

            if(line == "ref"){
                remove_faculty();
                return;
            }

            if(line == "res"){
                remove_student();
                return;
            }

            if(line == "rec"){
                remove_course();
                return;
            }

            if(line == "asc"){
                add_student_to_course();
                return;
            }

            if(line == "rsc"){
                remove_student_from_course();
                return;
            }

            if(line == "afc"){
                add_faculty_courses();
                return;
            }

            if(line == "rfc"){
                remove_faculty_courses();
                return;
            }
    }

};

int login(string &in_uname)
{
    cout << "Enter username." << endl;
    cin >> in_uname;

    string password;
    int status = get_value("Passwords", in_uname, aims + "pass.txt", password);

    switch(status){
        case -1 : cout << "Fatal error." << endl;
            exit(-1);
        case 1 : cout << "Invalid username. Try again." << endl;
            return -1;
        case 0 : 
            cout << "Enter password." << endl;
            string in_pw;
            cin >> in_pw;
            if(in_pw != password){
                cout << "Invalid password." << endl;
                return -1;
            }
            else cout << "Welcome. Creating instance..." << endl;
        break;
    }

    switch(in_uname[0]){
        case 'a' : return 0;
        case 'f' : return 1;
        case 's' : return 2;
        default : return -1;
    }
    
}

void make_id(string &out)
{
    string tmp;
    cout << "Enter branch." << endl;
    cin >> tmp;
    out += tmp;

    cout << "Enter year of joining." << endl;
    cin >> tmp;
    out += to_string(stoi(tmp) % 100);

    cout <<  "Enter programme." << endl;
    cin >> tmp;
    out += tmp;

    cout << "Enter roll number." << endl;
    cin >> tmp;
    out += tmp;

}

void Admin :: create_student()
{
    string student_id = "s_";
    make_id(student_id);

    string dummy = "password";

    int status = add_key("Passwords", student_id, aims + "pass.txt", dummy);
    
    if(status == 1){
        cout << "Already exists." << endl;
        return;
    }
    
    
    cout << "Enter student name." << endl;

    string new_fname = aims + "Students/" + student_id;
    ofstream new_f;
    new_f.open(new_fname);

    if(!new_f){
        cout << "File creation error. Try again." << endl;
        return;
    }

    string tmp;
    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    new_f << "[Data]" << endl;
    new_f << "> Name : " << name << endl;
    new_f << "> ID : " << student_id << endl;

    cout << "Enter current semester." << endl;
    cin >> tmp;
    new_f << "> Current semester : " << tmp << endl;
    add_key(tmp, student_id, aims + "Students/list", "");

    cout << "Enter mail address." << endl;
    cin >> tmp;
    new_f << "> Email ID : " << tmp << endl;

    new_f << "[Courses]" << endl;

    cout << "Created. \nUsername : " + student_id + "\nPassword : password" << endl;

}

void Admin :: remove_student()
{
    cout << "Enter student ID." << endl;
    string key;
    cin >> key;

    cout << "Enter current semester." << endl;
    string sem;
    cin >> sem;

    if(key.find("s_") != 0){
        key.insert(0, "s_");
    }

    int status = set_value("Passwords", key, aims + "pass.txt", "", 1);
    
    if(status == 1){
        cout << "No such student.";
        return;
    }

    status = set_value(sem, key, aims + "Students/list", "", 1);

    if(status == 1){
        cout << "No such student.";
        return;
    }

    string fname = aims + "Students/" + key;
    remove(fname.c_str());

    cout << "Removed." << endl;
}

void Admin :: create_faculty()
{
    string fac_id = "f_";
    string tmp;

    cout << "Enter faculty department." << endl;
    cin >> tmp;
    fac_id += tmp;
    
    cout << "Enter faculty number." << endl;
    cin >> tmp;
    fac_id += tmp;

    string dummy = "12345678";

    int status = add_key("Passwords", fac_id, aims + "pass.txt", dummy);
    
    if(status == 1){
        cout << "Already exists." << endl;
        return;
    }

    cout << "Enter faculty name." << endl;
    string new_fname = aims + "Faculty/" + fac_id;
    ofstream new_f;
    new_f.open(new_fname);

    if(!new_f){
        cout << "File creation error. Try again." << endl;
        return;
    }

    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    add_key("", fac_id, aims + "Faculty/list", name);

    new_f << "[Data]" << endl;
    new_f << "> Name : " << name << endl;
    new_f << "> ID : " << fac_id << endl;

    cout << "Enter email address." << endl;

    cin >> tmp;
    new_f << "> Email ID : " << endl;

    new_f << "[Courses]" << endl;

    cout << "Created. \nUsername :" + fac_id + "\nPassword : 12345678" << endl;
    return;
}

void Admin :: remove_faculty()
{
    cout << "Enter faculty ID." << endl;
    string key;
    cin >> key;

    if(key.find("f_") != 0){
        key.insert(0, "f_");
    }

    int status = set_value("Passwords", key, aims + "pass.txt", "", 1);
    
    if(status == 1){
        cout << "No such member.";
        return;
    }
    set_value("", key, aims + "Faculty/list", "", 1);

    string fname = aims + "Faculty/" + key;
    remove(fname.c_str());

    cout << "Removed." << endl;
}

void Admin :: create_course()
{
    cout << "Enter course name." << endl;
    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    cout << "Enter semester of course." << endl;
    string sem;
    cin >> sem;

    cout << "Enter course ID." << endl;
    string id;
    cin >> id;
    
    fstream f;
    f.open(aims + "Courses/" + sem + "/" + id, ios_base::in);
    if(f){
        cout << "Already exists." << endl;
        f.close();
        return;
    }
    add_key(sem, id, aims + "Courses/list", name);
    f.open(aims + "Courses/" + sem + "/" + id, ios_base::out);

    if(!f){
        cout << "File creation error. Try again." << endl;
        return;
    }

    f << "[Data]" << endl;
    f << "> Name : " + name << endl;
    
    cout << "Enter credits." << endl;
    string cred;
    cin >> cred;
    f << "> Credits : " + cred << endl;
    f << "> Status : ungraded" << endl;

    f << "> Instructor : " << endl;

    f << "[Students]" << endl;

    f.close();
    cout << "Created." << endl;
}

void Admin :: remove_course()
{
    cout << "Enter course ID." << endl;
    string key;
    cin >> key;

    cout << "Enter semester." << endl;
    string sem;
    cin >> sem;

    int status = set_value(sem, key, aims + "Courses/list", "", 1);
    
    if(status == 1){
        cout << "No such course." << endl;
        return;
    }

    string alt_fn = aims + "Students/";
    vector <string> students;
    string out = aims + "Courses/" + sem + "/" + key;
    bool stat = dump_keys(students, "Students", out);

    for(int i = 0; i < students.size(); i++)
        set_value("Courses", key, alt_fn + students[i],"", 1);

    string fac;
    get_value("Data", "Instructor", out, fac);
    set_value("Courses", key, aims + "Faculty/" + fac, "", 1);
    
    remove(out.c_str());

    cout << "Removed." << endl;
}

void Admin :: remove_student_from_course()
{
    cout << "Enter student ID." << endl;
    string st_id;
    cin >> st_id;

    if(st_id.find("s_") != 0){
        st_id.insert(0, "s_");
    }

    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    string sem;
    if(get_value("Data", "Current semester", aims + "Students/" + st_id, sem) == -2){
        cout << "No such student." << endl;
        return;
    }
    switch(set_value("Students", st_id, aims + "Courses/" + sem + "/" + cr_id, "", 1)){
        case -2:
        cout << "No such course." << endl;
        return;

        case 1:
        cout << "Student not in course." << endl;
        return;
    }

    set_value("Courses", cr_id, aims + "Students/" + st_id, "", 1);
    

}

void Admin :: add_student_to_course()
{
    cout << "Enter student ID." << endl;
    string st_id;
    cin >> st_id;

    if(st_id.find("s_") != 0){
        st_id.insert(0, "s_");
    }

    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    string sem;
    if(get_value("Data", "Current semester", aims + "Students/" + st_id, sem) == -2){
        cout << "No such student." << endl;
        return;
    }


    add_key("Courses", cr_id, aims + "Students/" + st_id, "ungraded");
    int x = add_key("Students", st_id, aims + "Courses/" + sem + "/" + cr_id, "ungraded");
    switch(x){
        case -2:
        cout << "No such course.";
        return;

        case 1:
        cout << "Student already in course." << endl;
        return;
    }
}

void Admin :: add_faculty_courses()
{
    cout << "Enter faculty ID." << endl;
    string fac_id;
    cin >> fac_id;

    if(fac_id.find("f_") != 0){
        fac_id.insert(0, "f_");
    }

    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    cout << "Enter semester." << endl;
    string sem;
    cin >> sem;

    fstream f;
    f.open(aims + "Courses/" + sem + "/" + cr_id, ios_base::in);
    if(!f){
        cout << "No such course exists." << endl;
        return;
    }
    f.close();
    
    switch(add_key("Courses", cr_id, aims + "Faculty/" + fac_id, sem)){
        case -2:
        cout << "No such member." << endl;
        return;

        case 1:
        cout << "Course already offerred by faculty." << endl;
        return;
    }

    set_value("Data", "Instructor", aims + "Courses/" + sem + "/" + cr_id, fac_id);
}

void Admin :: remove_faculty_courses()
{
    cout << "Enter faculty ID." << endl;
    string fac_id;
    cin >> fac_id;

    if(fac_id.find("f_") != 0){
        fac_id.insert(0, "f_");
    }

    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    switch(set_value("Courses", cr_id, aims + "Faculty/" + fac_id, "", 1)){
        case -2:
        cout << "No such member." << endl;
        return;

        case 1:
        cout << "No such course offerred by member." << endl;
        return;
    }
}

void Admin :: view_courses()
{
    cout << "Enter semester." << endl;
    string sem;
    cin >> sem;
    if(sem == "all") dump_file(aims + "Courses/list");
        
    else dump_section(aims + "Courses/list", sem);
}

void Admin :: view_faculty()
{
    dump_section(aims + "Faculty/list", "");
}

void Admin :: view_students()
{
    cout << "Enter semester." << endl;
    string sem;
    cin >> sem;
    if(sem == "all") dump_file(aims + "Students/list");
        
    else dump_section(aims + "Students/list", sem);
}



void Student :: reg_course()
{
    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    string tmp;

    if(get_value("Data", "Instructor", aims + "Courses/" + to_string(current_sem) + "/" + cr_id, tmp) == -2){
        cout << "No such course floated in this semester." << endl;
        return;
    }

    if(tmp == ""){
        cout << "Faculty not assigned to course. Attempt registration later." << endl;
        return;
    }

    if((get_value("Data", "Status", aims + "Courses/" + to_string(current_sem) + "/" + cr_id, tmp) || true) && tmp == "graded"){
        cout << "Course already graded." << endl;
        return;
    }

    //if(get_value("Courses", cr_id, aims + "Faculty/" + tmp, tmp) == 1) will be the semester : unnecessary.

    add_key("Students", id, aims + "Courses/" + to_string(current_sem) + "/" + cr_id, "ungraded");
    switch(add_key("Courses", cr_id, aims + "Students/" + id, "ungraded")){
        case 1:
        cout << "Student has already registered for course." << endl;
        return;
        case 0:
        cout << "Successfully registered." << endl;
        return;
    }
}

void Student :: dereg_course()
{
    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    string tmp;
    if(get_value("Courses", cr_id, aims + "Student/" + id, tmp) == 1){
        cout << "No such course registered.";
        return;
    }

    if(tmp != "ungraded"){
        cout << "Course already graded. Cannot deregister." << endl;
        return;
    }

    set_value("Courses", cr_id, aims + "Student/" + id, "", 1);

    set_value("Students", id, aims + "Courses/" + to_string(current_sem) + "/" + cr_id, "", 1);
} 

void Student :: view_all_courses()
{
    dump_section(aims + "Courses/list", to_string(current_sem));
}

void Student :: view_grade()
{
    dump_file(aims + "Students/" + id);
}



void Faculty :: view_courses()
{
    dump_section(aims + "Faculty/" + id, "Courses");
}

void Faculty :: view_students_in_course()
{
    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    string sem;
    int stat = get_value("Courses", cr_id, aims + "Faculty/" + id, sem);

    if(stat == 1){
        cout << "Course not offered by member." << endl;
        return;
    }

    dump_section(aims + "Courses/" + sem + "/" + cr_id, "Students");
}

void Faculty :: assign_grade()
{
    cout << "Enter course ID." << endl;
    string cr_id;
    cin >> cr_id;

    string sem;
    int stat = get_value("Courses", cr_id, aims + "Faculty/" + id, sem);

    if(stat == 1){
        cout << "Course not offered by member." << endl;
        return;
    }

    cout << "Enter student ID." << endl;
    string st_id;
    cin >> st_id;

    if(st_id.find("s_") != 0){
        st_id.insert(0, "s_");
    }

    string gr;
    stat = get_value("Students", st_id, aims + "Courses/" + sem + "/" + cr_id, gr);
    if(stat == 1){
        cout << "Student not in course." << endl;
        return;
    }
    if(gr != "ungraded"){
        cout << "Student already graded for course. Do you wish to change it? \n y / n" << endl;
        string choice;
        cin >> choice;

        if(choice == "n") return;
    }

    cout << "Enter grade." << endl;
    cin >> gr;
    set_value("Students", st_id, aims + "Courses/" + sem + "/" + cr_id, gr);
    set_value("Courses", cr_id, aims + "Students/" + st_id, gr);

}


int main()
{
    bool logg = false;
    string line;
    User * current_user;
    int usr_stat = -1;
    string in_id;
    cout << "Welcome. Enter \"login\" to continue. Enter \"help\" for a list of commands, and \"exit\" to terminate the program." << endl;

    while(cout << "\n\n>>> " && cin >> line){
        //termination command
        if(line == "exit"){
            delete current_user;
            return 0;
        }

        cout << "\n\n";

        //command overview
        if(line == "help"){
            current_user->view_cmd(usr_stat);
            continue;
        }

        //checks for login-logout status
        if(!logg && line != "login"){
            cout << "No user logged in. Cannot act. Enter command : login." << endl;
            continue;
        }
        if(logg && line == "login"){
            cout << "Other user logged in. Do you wish to log out? \n y / n" << endl;

            string choice;
            cin >> choice;

            if(choice == "n") continue;

            logg = false;
            delete current_user;
            continue;
        }

        //creates object
        if(line == "login"){
            usr_stat = login(in_id);
            if(usr_stat != -1) logg = true;

            switch(usr_stat){
                case 1 : 
                    current_user = new Faculty(in_id);
                    break;
                case 0 :
                    current_user = new Admin();
                    break;
                case 2 :
                    current_user = new Student(in_id);
                    break;
                default : return 1;
            }
            //cout << "Done." << endl;
            continue;
        }

        //deletes object
        if(line == "logout"){
            delete current_user;
            logg = false;
            usr_stat = -1;
            continue;
        }

        //changing password
        if(line == "ch"){
            current_user->set_password();
            continue;
        }

        //checks for existence of commands
        string cmd;
        if(line.length() > 3){
            if(get_key(to_string(usr_stat),  cmd, aims + "commands", line) == 0)
                line = cmd;
            else{
                cout << "Invalid command." << endl;
                continue;
            }
        }
        else{
            if(get_value(to_string(usr_stat), line, aims + "commands", cmd) == 1){
                cout << "Invalid command." << endl;
                continue;
            }
        }
        
        string tmp;
        if(get_value(to_string(usr_stat), line, aims + "commands", tmp) == 1){
            cout << "Invalid command." << endl;
            continue;
        }

        
        //function call multiplexer - Actual command execution.
        current_user->multiplexer(line);
    
    }
}

