#!/bin/bash

echo -e "-----Welcome to jrem cmd installation-----\n"
echo -e "--Verifying make and gcc\n"
if ! command -v gcc &> /dev/null 
then
	echo "gcc could not be found please install gcc and retry\n"
	exit
elif ! command -v make &> /dev/null
then
	echo "make could not be found please install make and retry\n"
	exit
else
	echo -e "make and gcc present on your system\n"
	if make
	then
		dst_path=$HOME/.local/bin/
		echo -e "compilation done succes...\nMoving binary file to ${dst_path}\n"
		if [ ! -d "$dst_path" ]; then
			echo -e "path ${dst_path} null creating one"
			mkdir "$dst_path"
		fi
		mv jrem $HOME/.local/bin/
		echo "Done you can now use jrem\n"
		echo "Do not forget to add ${dst_path} to your class path"
	else
		echo -e "Compilation error operation aborted\n"
	fi
fi
echo -e "---------------------Script Over-------------------------------"
