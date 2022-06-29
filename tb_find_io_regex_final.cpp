#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <fstream>
#include <utility>


using namespace std;

string get_name_module(ifstream &file){
    file.seekg(0,ios::beg);
    string s;
    smatch sm;
    regex re_modulename ("\\s*module\\s+(\\w+)\\s*\\(?$"); // module name ($2)
    while(!file.eof()){
        getline(file,s);
        if(regex_match(s,re_modulename)){            
            regex_search(s,sm,re_modulename);            
            return sm.str(1);
            break;
        }
    }
    return "";


}
void delete_comments(ifstream &file, vector<string> &vs){
    //delete the comments
    string linea;
    bool final_flag=false;    
    file.seekg(0,ios::beg);
    while(!file.eof()){
        getline(file,linea);
        regex re_startmodule ("(.*)(\\()(.*)"); // module name ( .....
        regex re_commments0 ("(.*)(\\/\\/.*)"); // comments: //
        regex re_commments1 ("(.*)(\\/\\*.*\\*\\/)(.*)"); // comments: /* */ same line
        regex re_commments2 ("(\\/\\*)(.*)()"); // comments: /* 
        regex re_commments3 (".*(\\*\\/)(.*)"); // comments: */ 
        //regex re_endmodule ("\\)\\s*;"); // comments: */ 
        regex re_endmodule ("(.*)(\\)\\s*;)"); // )  ;
        regex re_invalid ("^\\s*$"); // line without valid characters (only spaces)
        
//        while(!regex_match(linea,re_startmodule)){
 //           getline(file,linea);
  //      }
        linea = regex_replace(linea,re_startmodule,"$3");
        linea = regex_replace(linea,re_commments0,"$1");
        linea = regex_replace(linea,re_commments1,"$1$3");
        if(regex_match(linea,re_commments2)){
            linea = regex_replace(linea,re_commments2,"$3");
            while(!regex_match(linea,re_commments3)){
                getline(file,linea);
            }
        }
        linea = regex_replace(linea,re_commments3,"$2");
        if(regex_match(linea,re_endmodule)){
            linea=regex_replace(linea,re_endmodule,"$1");
            final_flag=true;
        }        
        if(linea != "" & !regex_match(linea,re_invalid)){
            vs.push_back(linea);
            
        }
        if(final_flag){
                break;
            }
        
    }
}
void delete_white_spaces(vector<string> &vs){
    string aux;
    regex re_lrspaces ("(\\s*)(.+,)(\\s*)"); //white spaces at begin and the end
    for(int i =0; i< vs.size(); i++){
        aux=vs[i];                
        aux=regex_replace(aux,re_lrspaces,"$2");
        vs[i]=aux;        
    }
}
void get_inputs_sb(vector <string> &vs,vector <string> &v_single_inputs){
     // entradas de un solo bit
    for(int i=0; i<vs.size();i++)
    {
        smatch sm;
        regex re("\\s*input\\s+(\\w+.*),?");
        regex_match(vs[i],sm,re);        
        string suf=sm.str(1);
        int nLeft=suf.size();
        regex re1("\\s*,?\\s*(\\w+)(.*)");
        while (nLeft>0)
        {
            regex_match(suf,sm,re1);
            if(sm.str(1)!=""){
                v_single_inputs.push_back(sm.str(1));
            }
            suf=sm.str(2);
            nLeft=suf.size();
        }
    
    }
}
void get_outputs_sb(vector <string> &vs,vector <string> &v_single_outputs){
     // entradas de un solo bit
    for(int i=0; i<vs.size();i++)
    {
        smatch sm;
        regex re("\\s*output\\s+(\\w+.*),?");
        regex_match(vs[i],sm,re);
        string suf=sm.str(1);
        int nLeft=suf.size();
        regex re1("\\s*,?\\s*(\\w+)(.*)");
        while (nLeft>0)
        {
            regex_match(suf,sm,re1);
            if(sm.str(1)!=""){
                v_single_outputs.push_back(sm.str(1));
            }
            suf=sm.str(2);
            nLeft=suf.size();
        }
    
    }
}
void get_inputs_bus(vector <string> &vs, vector <pair<string,int>> &v_inputs_bus){
    for(int i=0; i<vs.size();i++)
    {
        smatch sm;
        regex re("\\s*input\\s*\\[\\s*(\\d+)\\s*:\\s*(\\d+)\\s*\\]\\s*(\\w+.*)");
        if(regex_match(vs[i],re)){
            regex_match(vs[i],sm,re);
            string suf=sm.str(3);
            int num= (stoi(sm.str(1)) > stoi(sm.str(2))) ?  stoi(sm.str(1)): stoi(sm.str(2)) ;
            int nLeft=suf.size();
            regex re1("\\s*,?\\s*(\\w+)(.*)");
            while (nLeft>0)
            {
                regex_match(suf,sm,re1);
                if(sm.str(1)!=""){
                    v_inputs_bus.push_back(make_pair(sm.str(1),num+1));
                }
                suf=sm.str(2);
                nLeft=suf.size();
            }
        }
        
    
    }
}
void get_outputs_bus(vector <string> &vs, vector <pair<string,int>> &v_outputs_bus){
    for(int i=0; i<vs.size();i++)
    {
        smatch sm;
        regex re("\\s*output\\s*\\[\\s*(\\d+)\\s*:\\s*(\\d+)\\s*\\]\\s*(\\w+.*)");
        if(regex_match(vs[i],re)){
            regex_match(vs[i],sm,re);
            string suf=sm.str(3);
            int num= (stoi(sm.str(1)) > stoi(sm.str(2))) ?  stoi(sm.str(1)): stoi(sm.str(2)) ;
            int nLeft=suf.size();
            regex re1("\\s*,?\\s*(\\w+)(.*)");
            while (nLeft>0)
            {
                regex_match(suf,sm,re1);
                if(sm.str(1)!=""){
                    v_outputs_bus.push_back(make_pair(sm.str(1),num+1));
                }
                suf=sm.str(2);
                nLeft=suf.size();
            }
        }
        
    
    }
}

///////////////////// CLASS DEFINITION FOR VERILOG MODULE //////////////////////////
class VerilogModule {

public:
    string name;
    vector <pair<string, int>> bus_inputs;
    vector <pair<string, int>> bus_outputs;
    vector <string> singleBit_inputs;
    vector <string> singleBit_outputs;

};

string CreateForLoop (string var, int num) {
    string forLoop = "\n";
    forLoop += "/*\n";
    forLoop += "  for ( i=0 ; i<2**" + to_string(num) + " ; i=i+1 )\n";
    forLoop += "  begin\n";
    forLoop += "     " + var + "_tb" +" = i; \n     #1;\n";
    forLoop += "  end\n";
    forLoop += "*/\n";
    return forLoop;
}


int main(){
    VerilogModule myModule1;

    ifstream file;
    vector <string> vs;
    vector <string> v_single_inputs,v_single_outputs;//vectores para almacenar las entradas y las salidas (1 bit)
    vector <pair<string,int>> v_inputs_bus, v_outputs_bus; //vectores para almacenar las entradas y las salidas (buses)
    string module_name;
    file.open("test.v",ios::in);
    if(file.fail()){
        cout << "Error";
        std::exit(EXIT_FAILURE);
    }
    module_name = get_name_module(file);
    myModule1.name = module_name; //assign the name 
    delete_comments(file,vs);
    delete_white_spaces(vs);

    get_inputs_sb(vs,myModule1.singleBit_inputs);
    get_outputs_sb(vs,myModule1.singleBit_outputs);

    get_inputs_bus(vs,myModule1.bus_inputs);
    get_outputs_bus(vs,myModule1.bus_outputs);
        

    file.close();


    //////////////// WRITE TEST BENCH INPUTS & OUTPUTS TO A FILE ////////////////

    string regs = "\n";
    string wires = "\n";
    string uut = "\n" + myModule1.name + " UUT( ";
    string assignations = "\n";
    string sbiConc = "{ "; //single bit inputs concatenated
    string busIterations;

    for (string auxString: myModule1.singleBit_inputs){
        regs += " reg\t" + auxString + "_tb;\n";
        uut += "\n\t\t\t." + auxString + "\t( " + auxString +"_tb ),";
        assignations += " " + auxString + "_tb\t=\t1'b0;\n";
        sbiConc += auxString + "_tb, ";
    }

    for (pair auxPair: myModule1.bus_inputs){
        regs += " reg [" + to_string(auxPair.second-1) + ":0]\t" + auxPair.first + "_tb;\n";
        uut += "\n\t\t\t." + auxPair.first + "\t( " + auxPair.first +"_tb ),";
        assignations += " " + auxPair.first + "_tb\t=\t" + to_string(auxPair.second) + "'b" + string(auxPair.second, '0') + ";\n";
        busIterations += CreateForLoop(auxPair.first, auxPair.second);
    }

    for (string auxString: myModule1.singleBit_outputs){
        wires += " wire\t" + auxString + "_tb;\n";
        uut += "\n\t\t\t." + auxString + "\t( " + auxString +"_tb ),";
    }

    for (pair auxPair: myModule1.bus_outputs){
        wires += " wire [" + to_string(auxPair.second-1) + ":0]\t" + auxPair.first + "_tb;\n";
        uut += "\n\t\t\t." + auxPair.first + "\t( " + auxPair.first +"_tb ),";
    }

    uut.pop_back();
    uut.pop_back();
    uut += ")  );\n";

    string sbiConcIterations;
    if (!myModule1.singleBit_inputs.empty()){
        sbiConc.pop_back();
        sbiConc.pop_back();
        sbiConc += " }";
        sbiConcIterations = CreateForLoop (sbiConc, myModule1.singleBit_inputs.size());
    }

    fstream myFile;
    myFile.open(myModule1.name + "_tb.sv", ios::out);
    if (myFile.is_open()) {

        myFile << "`timescale 1ns/1ns\n\n";
        myFile << "module " << myModule1.name + "_tb;\n";
        myFile << regs;
        myFile << wires;
        myFile << "\n integer i;\n";
        myFile << uut;
        myFile << "\ninitial begin\n";
        myFile << "\n $dumpvars( 1, " + myModule1.name + "_tb );";
        myFile << "\n $dumpfile( \"dump.vcd\" );\n";
        myFile << assignations;
        myFile << "\n #1;\n";
        myFile << sbiConcIterations;
        myFile << busIterations;
        myFile << "\n $finish();\n";
        myFile << "\n end";
        myFile << "\nendmodule";

        myFile.flush();
        myFile.close();
    }

    return 0;
}
