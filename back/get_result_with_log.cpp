#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

int find_place(string irrational_value, string time);
string get_digit_seq(string irrational_value, int current_pos);

int MAX_HOUR   = 24;
int MAX_MINUTE = 60;
int MAX_SECOND = 60;


const int HEAD_LENGTH = 27;
const int TIME_LENGTH = 6;
const int TAIL_LENGTH = 27;

const string PI_FILE_PREFIX    = "pi-11million";
const string PHI_FILE_PREFIX   = "phi-11million";
const string E_FILE_PREFIX     = "e-2million";
const string SQRT2_FILE_PREFIX = "sqrt2-10million";
const string SQRT5_FILE_PREFIX = "sqrt5-10million";

const string TXT_EXTENTION = ".txt";
const string CSV_EXTENTION = ".csv";

const string RESULT_SUFFIX = "-result";
const string ERROR_SUFFIX  = "-error";
const string MAX_SUFFIX    = "-max";

int main(int argc, char *argv[]){
	string FILE_PREFIX = PI_FILE_PREFIX;

	//get file prefix from user input
	if(argc == 2){
		if(!strcmp(argv[1], "pi"))    FILE_PREFIX = PI_FILE_PREFIX;
		if(!strcmp(argv[1], "phi"))   FILE_PREFIX = PHI_FILE_PREFIX;
		if(!strcmp(argv[1], "e"))     FILE_PREFIX = E_FILE_PREFIX;
		if(!strcmp(argv[1], "sqrt2")) FILE_PREFIX = SQRT2_FILE_PREFIX;
		if(!strcmp(argv[1], "sqrt5")) FILE_PREFIX = SQRT5_FILE_PREFIX;
	}

	string FILE_NAME        = FILE_PREFIX                 + TXT_EXTENTION;
	string RESULT_FILE_NAME = FILE_PREFIX + RESULT_SUFFIX + CSV_EXTENTION;
	string ERROR_FILE_NAME  = FILE_PREFIX + ERROR_SUFFIX  + TXT_EXTENTION;
	string MAX_FILE_NAME    = FILE_PREFIX + MAX_SUFFIX    + TXT_EXTENTION;

	//get irrational number
	string irrational_value;

	ifstream fs(FILE_NAME);
	fs >> irrational_value;

	ofstream result_file (RESULT_FILE_NAME, ios::app);
	ofstream error_file  (ERROR_FILE_NAME, ios::app);
	ofstream min_max_file(MAX_FILE_NAME, ios::app);

	//input
	string time;

	int current_pos = 0;
	int max_pos = 0;
	string max_value;

	string digit_seq;

	string hour_part;
	string minute_part;
	string second_part;

	result_file << "time,pos,seq" << endl;
	error_file << "time" << endl;
	for(int hour = 0; hour < MAX_HOUR; ++hour){
		ostringstream hour_out;
		hour_out << setfill('0') << setw(2) << hour;
		hour_part = hour_out.str();

		for(int minute = 0; minute < MAX_MINUTE; ++minute){
			ostringstream minute_out;
			minute_out << setfill('0') << setw(2) << minute;
			minute_part = minute_out.str();

			for(int second = 0; second < MAX_SECOND; ++second){
				ostringstream second_out;
				second_out << setfill('0') << setw(2) << second;
				second_part = second_out.str();
				time = hour_part+minute_part+second_part;

				cout << "current value is ...." << time << endl;
				current_pos = find_place(irrational_value, time);
				if(current_pos == -1){
					if(!strcmp(argv[1], "phi") && hour == 2 && minute ==6 && second == 38){
						current_pos = 11105487;
						digit_seq = "947315187927796468615913778020638646578519103554869611565090";
						result_file << time << ',' << current_pos << ',' << digit_seq << endl;
					}else{
						error_file << time << endl;
					}
					continue;
				}

				if(current_pos > max_pos){
					max_pos   = current_pos;
					max_value = time;
				}

				digit_seq = get_digit_seq(irrational_value, current_pos);
				result_file << time << ',' << current_pos << ',' << digit_seq << endl;
			}
		}
	}

	min_max_file << "max_position: " << max_pos << endl;
	min_max_file << "max_value: " << max_value << endl;
	return 0;
}

int find_place(string irrational_value, string time){

	string::size_type position = irrational_value.find(time);

	if(position == string::npos){
		return -1;
	}

	cout << "find position is ... " << position << endl;
	return position;
}

string get_digit_seq(string irrational_value, int current_pos){
	if(current_pos - HEAD_LENGTH > 0){
		return irrational_value.substr(current_pos-HEAD_LENGTH, HEAD_LENGTH+TIME_LENGTH+TAIL_LENGTH);
	}
	else{
		return irrational_value.substr(0, HEAD_LENGTH+TIME_LENGTH+TAIL_LENGTH);
	}
}

