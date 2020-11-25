#include <iostream>
#include <sstream>
#include "Editor.h"

using namespace std;

Editor::Editor()
{
    buffer = new Buffer();
}
Editor::~Editor()
{
    // TODO: Implement destructor
    if(buffer!=nullptr){
    delete buffer;
    }
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        } catch (const out_of_range &oor) {
            cout << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            cout << "? " << re.what() << endl;
        }
    }
}
void Editor::cmdAppend()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // TODO: finish cmdAppend.
    while(true){
        string text;
        getline(cin,text);
        if(text=="."){
            break;
        }
        buffer->appendLine(text);
    }
}

void Editor::cmdInsert()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (true)
    {
        string text;
        getline(cin, text);
        if (text == ".")
            break;
        if (firstLine) {
            buffer->insertLine(text);
            firstLine = false;
        }  else {
            buffer->appendLine(text);
        }
    }
}

void Editor::cmdDelete(int start, int end)
{
    buffer->deleteLines(start, end);
}

void Editor::cmdNull(int line)
{
    cout << buffer->moveToLine(line) << endl;
}

void Editor::cmdNumber(int start, int end)
{
    buffer->showLines(start, end);
}

void Editor::cmdWrite(const string &filename)
{
    buffer->writeToFile(filename);
}

void Editor::dispatchCmd(const string &cmd)
{
    if (cmd == "a") {
        cmdAppend();
        return;
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    if(cmd[0]=='w' && cmd.size()==1){
        throw "Filename not specified";
    }
    if (cmd[0] == 'w' && cmd[1] == ' ') {
        // TODO: call cmdWrite with proper arguments
        if(cmd.size()==2){
           throw "Filename not specified";
        }
        const string filename=cmd.substr(2);
        cmdWrite(filename);
        return;
    }
    // TODO: handle special case "1,$n".
    if(cmd=="1,$n"){
        cmdNumber(1,-1);
        return;
    }
    if(cmd=="1,$d"){
        cmdDelete(1,-1);
        return;
    }
    int start, end;
    char comma, type = ' ';
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }
    ss >> comma >> end >> type;
    if (ss.good()) {
        if (type == 'n') {
            if(start>end){
                throw "Number range error";
            }
            cmdNumber(start, end);
            return;
        } else if (type == 'd') {
            if(start>end){
                throw "Delete range error";
            }
            cmdDelete(start, end);
            return;
        }
    }
    throw "Bad/Unknown command";
}
