#include "Student.h"
#include <string>
#include <sstream>
#include "Class.h"

std::string Student::toString() const
{
    // TODO: uncomment the following code after implementing class Student.

    std::ostringstream oss;
    oss << "Student Information:"
        << "\n\tid: " << id
        << "\n\tname: " << name
        << "\n\tenrollment year: " << year
        << "\n\tdegree: " << (degree == graduate ? "graduate" : "undergraduate")
        << std::endl;
    return oss.str();

    return "";
}

// TODO: implement functions which are declared in Student.h.

void Student::addClass(Class *c){
    this->classes.push_back(c);
}

double Graduate::getGrade(){    
    int point_sum=0;
    double sum=0.0;
    for (std::vector<Class *>::iterator it=classes.begin();it!=classes.end();++it){
        point_sum+=(*it)->point;
        double score=(*it)->getStudentWrapper(this->id).getScore();
        if(score>=90&&score<=100){
            sum+=4.0*(*it)->point;
        }
        else if(score>=80){
            sum+=3.5*(*it)->point;
        }
        else if(score>=70){
            sum+=3.0*(*it)->point;
        }
        else if(score>=60){
            sum+=2.5*(*it)->point;
        }
        else{
            sum+=2.0*(*it)->point;
        }
    }
    return sum/point_sum;
}

double Undergraduate::getGrade(){
    int point_sum=0;
    double grade_sum=0.0;
    for (std::vector<Class *>::iterator it=classes.begin();it!=classes.end();++it){
        point_sum+=(*it)->point;
        grade_sum+=(*it)->getStudentWrapper(this->id).getScore()/20.0*(*it)->point;
    }
    return grade_sum/point_sum;
}
