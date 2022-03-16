#!/bin/bash

echo -e "-----Welcome to jrem cmd installation-----\n"
echo -e "==>Verifying if make and gcc are installed...\n"
if ! command -v gcc &> /dev/null 
then
	echo "gcc could not be found. Please install gcc and retry.\n"
	exit
elif ! command -v make &> /dev/null
then
	echo "make could not be found. Please install make and retry.\n"
	exit
else
	echo -e "make and gcc are installed on your system.\n"
	if make
	then
		echo -e "Compilation done successfully...\nMoving binary file to $HOME/.local/bin/\n"
		mv jrem $HOME/.local/bin
		echo "Done! You can now enjoy using jrem\n"
	else
		echo -e "Compilation error! Operation aborted\n"
	fi
fi
echo -e "---------------------Script Over-------------------------------"
