#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

int find_place(string irrational_value, string time);
string get_digit_seq(int current_pos);

int MAX_HOUR   = 24;
int MAX_MINUTE = 60;
int MAX_SECOND = 60;


const int HEAD_LENGTH = 27;
const int TIME_LENGTH = 6;
const int TAIL_LENGTH = 27;


string PI_FILE_NAME    = "pi-11million.txt";
string PHI_FILE_NAME   = "phi-11million.txt";
string E_FILE_NAME     = "e-2million.txt";
string SQRT2_FILE_NAME = "sqrt2-10million.txt";

string PI_RESULT_FILE_NAME   = "pi-11million-result.csv";
string PHI_RESULT_FILE_NAME  = "phi-11million-result.csv";
string E_RESULT_FILE_NAME    = "e-2million-result.txt";
string SQRT2_RESULTFILE_NAME = "sqrt2-10million-result.txt";

string PHI_ERROR_FILE = "phi-11million-error.txt";

string PHI_MIN_MAX_FILE = "phi-11million-min-max.txt";

string pi;
string phi;
string e;
string sqrt2;

int main(){
	//get irrational number
	ifstream efs(E_FILE_NAME);
	efs >> e;
	ifstream pifs(PI_FILE_NAME);
	pifs >> pi;
	ifstream phifs(PHI_FILE_NAME);
	phifs >> phi;

	ofstream result_file (PHI_RESULT_FILE_NAME, ios::app);
	ofstream error_file  (PHI_ERROR_FILE, ios::app);
	ofstream min_max_file(PHI_MIN_MAX_FILE, ios::app);

	//input
	string irrational_value = "phi";
	string time;

	int current_pos = 0;
	int max_pos = 0;
	string max_value;
	int min_pos = INT_MAX;
	string min_value;

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
					error_file << time << endl;
					continue;
				}

				if(current_pos > max_pos){
					max_pos   = current_pos;
					max_value = time;
				}
				if(current_pos < min_pos){
					min_pos   = current_pos;
					min_value = time;
				}

				digit_seq = get_digit_seq(current_pos);
				result_file << time << ',' << current_pos << ',' << digit_seq << endl;
			}
		}
	}

	min_max_file << "min_position: " << max_pos << endl;
	min_max_file << "min_value: " << max_value << endl;
	min_max_file << "max_position: " << max_pos << endl;
	min_max_file << "max_value: " << max_value << endl;
	return 0;
}


int find_place(string irrational_value, string time){
	string find_value;

	//select value
	if(irrational_value == "pi")     find_value = pi;
	if(irrational_value == "phi")    find_value = phi;
	if(irrational_value == "e")      find_value = e;
	if(irrational_value == "sqrt2")  find_value = sqrt2;

	string::size_type position = find_value.find(time);

	if(position == string::npos){
		return -1;
	}

	cout << "find position is ... " << position << endl;
	return position;
}

string get_digit_seq(int current_pos){
	if(current_pos - HEAD_LENGTH > 0){
		return pi.substr(current_pos-HEAD_LENGTH, HEAD_LENGTH+TIME_LENGTH+TAIL_LENGTH);
	}
	else{
		return pi.substr(0, HEAD_LENGTH+TIME_LENGTH+TAIL_LENGTH);
	}
}

