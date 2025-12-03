#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
// #include"./TokenType.cpp"


class Lox{
    static bool hadError;
    public:
    Lox(){
        hadError=false;
    }
    void run(std::string source){
        std::cout<<source;
    }
    
    public:
    static void error(int line,std::string message){
        report(line,"",message);
    }

    public:
    static void report(int line, std::string where, std::string message){
        hadError=true;
        std::cout<<"[line "<<line<<" ] Error"<<where<<": "<<message<<std::endl;
    }

    void runPrompt(){
        
        while(true){
            std::cout<<"> ";
            std::cout.flush();

            std::string line;
            if(!std::getline(std::cin,line))break;
            
            run(line);
            hadError=false;
        }
    }


    void runFile(std::string path){
        std::ifstream file(path);
        if(!file.is_open()){
            throw std::runtime_error("Could not open the file " + path);
        }
        std::stringstream buffer;
        buffer<<file.rdbuf();

        std::string contents=buffer.str();
        if(hadError)exit(-1);
        run(contents);
    }
};



int main(int argc, char* argv[]){
    Lox clox;
    if(argc>1){
        throw std::runtime_error("Usage: cpplox [script]");
    }else if(argc==1){
        clox.runFile(argv[0]);
    }else{
        clox.runPrompt();
    }

   return 0;
}