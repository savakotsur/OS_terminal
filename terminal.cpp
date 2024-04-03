#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

void signalHandler(int signal);
void childHandler(int /*signum*/);
void performLsAction(const string& args);
void performCatAction(const string& args);
void performNiceAction(const string& args);
void killAllProcesses(const string& args);
void suspendProcess(pid_t pid);
void resumeProcess(pid_t pid);
void performPsAction(const string& args);
void executeExternalCommand(const string& args);

int main() {
    signal(SIGINT, signalHandler); 
    signal(SIGCHLD, childHandler);

    string command;
    while (true) {
        usleep(500000);
        cout << "Enter a command: ";
        getline(cin, command);

        size_t space_pos = command.find(' ');
        string cmd, args;

        if (space_pos != string::npos) {
            cmd = command.substr(0, space_pos);
            args = command.substr(space_pos + 1);
        } else {
            cmd = command;
        }

        if (cmd == "ls") {
            performLsAction(args);
        } else if (cmd == "cat") {
            performCatAction(args);
        } else if (cmd == "nice") {
            performNiceAction(args);
        } else if (cmd == "killall") {
            killAllProcesses(args);
        } else if (cmd == "suspend") {
            pid_t pid = stoi(args);
            suspendProcess(pid);
        } else if (cmd == "resume") {
            pid_t pid = stoi(args);
            resumeProcess(pid);
        } else if (cmd == "ps") {
            performPsAction(args);
        } else if (cmd == "run") {
            executeExternalCommand(args);
        } else if (cmd == "exit") {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid command\n";
        }
    }

    return 0;
}

// Обработчик сигналов
void signalHandler(int signal) {
    if (signal == SIGINT) {
        cout << "\nReceived SIGINT. Exiting...\n";
        exit(0);
    }
}

// Обработчик сигнала SIGCHLD (завершение дочернего процесса)
void childHandler(int /*signum*/) {
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        std::cout << "Child process " << pid << " terminated\n";
    }
}

// Функция для выполнения "ls" действия
void performLsAction(const string& args) {
    cout << "Listing directory contents:\n";
    string command = "ls " + args;
    system(command.c_str());
}

// Функция для выполнения "cat" действия
void performCatAction(const string& args) {
    cout << "Reading your file:'\n'";
    string command = "cat " + args;
    system(command.c_str());
}

// Функция для выполнения "nice" действия
void performNiceAction(const string& args) {
    cout << "Performing a nice action...\n";
    string command = "nice " + args;
    system(command.c_str());
}

// Функция для завершения всех процессов
void killAllProcesses(const string& args) {
    cout << "Killing all \"" << args << "\" processes...\n";
    string command = "killall '" + args + "'";
    system(command.c_str());
}

// Функция для приостановки выполнения процесса
void suspendProcess(pid_t pid) {
    kill(pid, SIGSTOP);
    std::cout << "Process with PID " << pid << " suspended\n";
}

// Функция для продолжения выполнения процесса
void resumeProcess(pid_t pid) {
    kill(pid, SIGCONT);
    std::cout << "Process with PID " << pid << " resumed\n";
}

// Функция для просмотра процессов
void performPsAction(const string& args) {
    cout << "Performing ps action...\n";
    string command = "ps " + args;
    system(command.c_str());
}

//Функция для запуска внешней программы
void executeExternalCommand(const string& args) {
    pid_t pid = fork();
    if (pid == 0) { // Child process
        string tmpCmd = "/Applications/" + args + ".app/Contents/MacOS/" + args;
        execlp(tmpCmd.c_str(), args.c_str(), NULL);
        cerr << "Failed to execute external program\n";
        exit(1);
    } else if (pid < 0) { // Fork failed
        cerr << "Fork failed\n";
    }
}
