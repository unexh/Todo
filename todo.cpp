#include <iostream>
#include <stack>
#include <fstream>
#include <ctime>
using namespace std;

class todo_app{
    private:
        int task_pending=0;
        int task_done=0;
    public:
        todo_app(){
            fstream fio_todo,fio_done;
            fio_todo.open("todo.txt",ios::in);
            string traverse;
            while(fio_todo){
                getline(fio_todo,traverse);
                if(traverse!="")
                    task_pending+=1;
            }
            // cout << task_pending << endl;
            fio_done.open("done.txt",ios::in);
            while(fio_done >> traverse){
                getline(fio_done,traverse);
                if(traverse!="")
                    task_done+=1;
            }
            fio_todo.close();
            fio_done.close();
            // cout << task_done << endl;
        }

        void usage(){
            cout << "Usage :-" << endl;
            cout << "$ ./todo add \"todo item\"  # Add a new todo\n";
            cout << "$ ./todo ls               # Show remaining todos\n";
            cout << "$ ./todo del NUMBER       # Delete a todo\n";
            cout << "$ ./todo done NUMBER      # Complete a todo\n";
            cout << "$ ./todo help             # Show usage\n";
            cout << "$ ./todo report           # Statistics" << endl;
        }

        void add(string data){
            fstream fio_todo;
            fio_todo.open("todo.txt",ios::app);
            fio_todo << data << endl;
            cout << "Added todo: \"" << data  << '\"' << endl;
            fio_todo.close();
        }

        void ls(){
            if(task_pending==0){
                cout << "There are no pending todos!" << endl;
                return;
            }
            fstream fio_todo;
            fio_todo.open("todo.txt",ios::in);
            cout << "lisiting all the remaining todos" << endl;
            string data;
            stack<string> stack;
            while(fio_todo){
                getline(fio_todo,data);
                if(data!="")
                    stack.push(data);
            }
            while(!stack.empty()){
                if(stack.top()!="")
                    cout << '[' << stack.size() << "] " << stack.top() << endl;
                stack.pop();
            }
        }

        string del(int index,bool msg){
            if(index<=task_pending && index!=0){
                int counter=1;
                fstream fio_todo,fio_todo_temp;
                fio_todo.open("todo.txt",ios::in);
                fio_todo_temp.open("temp.txt",ios::out);
                string traverse,deleted;
                int adjusted_counter=0;
                while(fio_todo){
                    getline(fio_todo,traverse);
                    if(counter++ != index && traverse!=""){
                        fio_todo_temp << traverse << endl;
                        adjusted_counter++;
                    }else if(traverse!=""){
                        deleted=traverse;
                    }
                }
                task_pending=adjusted_counter;
                fio_todo.close();
                fio_todo_temp.close();
                remove("todo.txt");
                rename("temp.txt","todo.txt");
                if(msg)
                    cout << "Deleted todo #" << index << endl;
                return deleted;
            }else{
                if(msg)
                    cout << "Error: todo #" << index << " does not exist. Nothing deleted." << endl;
                return "";
            }
        }

        void done(int index){
            if(index<=task_pending && index!=0){
                string done=del(index,false);
                fstream fio_done;
                fio_done.open("done.txt",ios::app);
                time_t now = time(0);
                tm *ltm = localtime(&now);
                fio_done << "x " << 1900 + ltm->tm_year << '-' << 1 + ltm->tm_mon << '-' << ltm->tm_mday << " " << done << endl;
                task_done++;
                fio_done.close();
                cout << "Marked todo #"<< index << " as done." << endl;
            }else{
                cout << "Error: todo #" << index <<  " does not exist." << endl;
            }
        }

        void report(){
            time_t now = time(0);
            tm *ltm = localtime(&now);
            cout << 1900 + ltm->tm_year<< '-' << 1 + ltm->tm_mon << '-' << ltm->tm_mday;
            cout << " Pending : " << task_pending << " Completed : " << task_done << endl;
        }

        void show_error(){
            cout << "Error: Unknown"<< endl;
        }
};

int main(int argc, char* argv[])
{
    todo_app cur_session;
    
    if(argc <2){
        cur_session.usage();
    }else{
        string task=argv[1];
        if(task == "add"){
            if(argc<3){
                cout << "Error: Missing todo string. Nothing added!" << endl;
            }else{
                cur_session.add(argv[2]);
            }
        }else if(task == "ls"){
            cur_session.ls();
        }else if(task == "del"){
            if(argc<3){
                cout << "Error: Missing NUMBER for deleting todo." << endl;
            }else{
                cur_session.del(atoi(argv[2]),true);
            }
        }else if(task == "done"){
            if(argc<3){
                cout << "Error: Missing NUMBER for marking todo as done." << endl;
            }else{
                cur_session.done(atoi(argv[2]));
            }
        }else if(task == "help"){
            cur_session.usage();
        }else if(task == "report"){
            cur_session.report();
        }else{
            cur_session.show_error();
        }
    }
    
    return 0;
}
