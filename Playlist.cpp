#include <iostream>
#include <string>
#include "Playlist.h"
#include <fstream>
#include <Windows.h>
using namespace std;


Playlist::Playlist() 
{
	name.assign("");
	songs.assign("");
}
Playlist::Playlist(string _name) 
{
	songs = "";
	name.assign(_name);
	fstream file(name);
	file << "#EXTM3U \n" << endl;
	file.close();
}
Playlist::Playlist(string _directory, string _name)
{
	name.assign(_name);
	fstream file;
	file.open(name, fstream::out);
	file << "#EXTM3U \n" << endl;

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	_directory += "\\*";
	hFind = FindFirstFile(_directory.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		cout << "We couldn't find such directory" << endl;
	}
	else
	{
		char buffer[260];
		do
		{
			string FileName;
			strcpy(buffer, ffd.cFileName);
			FileName.assign(buffer);
			if (FileName.find(".mp3") != string::npos)
			{
				string help = _directory;
				file << "#EXTINF: ";
				file.close();
				char artist[30];
				help.pop_back();
				fstream test(help+=FileName);
				test.seekg(-95, test.end);
				test.read(artist, 30);
				test.close(); 
				file.open(name, fstream::app);
				(songs += help) += ";";
				file <<"artist "<< artist << endl << help << endl<<endl;
			}
				
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	file.close();

}
Playlist& Playlist:: operator-=(string _file) 
{
	
	if (songs.find(_file) != string::npos) 
	{
		songs.erase(songs.find(_file),_file.length()+1);
	}
	songsToFile(name);
	
	return *this;
}
Playlist& Playlist:: operator -(string _directory)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	_directory += "\\*";
	hFind = FindFirstFile(_directory.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		cout << "We couldn't find such directory" << endl;
	}
	else
	{
		char buffer[260];
		do
		{
			string FileName;
			strcpy(buffer, ffd.cFileName);
			FileName.assign(buffer);
			if (FileName.find(".mp3") != string::npos)
			{
				string help = _directory;
				help.pop_back();
				if (songs.find(help+=FileName) != string::npos)
				{
					songs.erase(songs.find(help), help.length() + 1);
				}
			}

		} while (FindNextFile(hFind, &ffd) != 0);
	}

	
	songsToFile(name);

	return *this;

}
void Playlist::songsToFile(string _name)
{
	fstream file;
	file.open(_name, fstream::out);
	file << "#EXTM3U \n" << endl;

	int posi = 0;
	do
	{
		string help = songs.substr(posi, songs.find(";", posi) - posi + 1);
		if (help.find(".mp3") != string::npos)
		{
			file << "#EXTINF: ";
			file.close();
			char artist[30];
			help.pop_back();
			fstream test(help);
			test.seekg(-95, test.end);
			test.read(artist, 30);
			test.close();
			file.open(name, fstream::app);
			file << "artist " << artist << endl << help << endl << endl;
		}

		posi = songs.find(";", posi) + 1;
	} while (songs.find(";", posi) != string::npos);

	file.close();
}
void Playlist::getSongs() 
{
	int posi = 0;
	do 
	{
		cout << songs.substr(posi, songs.find(";", posi)-posi) << endl;
		posi = songs.find(";", posi)+1;
	} while (songs.find(";", posi) != string::npos);
}
Playlist::~Playlist() 
{
	_fcloseall();
}
