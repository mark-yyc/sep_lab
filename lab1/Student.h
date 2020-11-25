#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>
#include <vector>

class Class;

enum Degree {
    undergraduate,
    graduate
};

class Student {
    // TODO: implement class Student.
private:
    std::string name;
    std::string year;
    Degree degree;
protected:
    std::vector<Class*> classes;
public:
    const std::string id;
    Student(const std::string &id,const std::string &name,const std::string  &year,const Degree &degree):id(id){
        this->name=name;
        this->year=year;
        this->degree=degree;
    }
    std::string toString() const;
    virtual double getGrade()=0 ;
    void addClass(Class *c);
};

// TODO: implement class Graduate.
class Graduate: public Student{
public:
    Graduate(const std::string &id,const std::string &name,const std::string &year):Student(id,name,year,graduate){
    }
    double getGrade();
};

// TODO: implement class Undergraduate.
class Undergraduate: public Student{
public:
    Undergraduate(const std::string &id,const std::string &name,const std::string &year):Student(id,name,year,undergraduate){}
    double getGrade();
};

class StudentWrapper {
private:
    const Student &student;
    double score = 0.0;
public:
    const std::string id;
    // TODO: fix error
    StudentWrapper(const std::string &id, const Student &student):student(student),id(id) {
    }

    void setScore(double score)
    {
        if (score < 0 || score > 100)
            throw "Invalid Score!";
        this->score = score;
    }

    double getScore() const
    {
        return this->score;
    }

    std::string toString() const
    {
        return student.toString();
    }
};

#endif // STUDENT_H_
