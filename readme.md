this is a test ftp client 

compile envir:
	ubuntu 14.04
	gcc4.8.4

usage:
	git clone git@github.com:wo-xiaotong/ftp-magic.git
	make
	./magic_ftp -h

user interface:
	1.noop:Send noop 
	2.pwd:Print current working directory 
	3.cd dir:Change working directory
	4.ls dir/file:Print dir or file info 
	5.mkdir dir:Create directory 
	6.rmdir dir:Remove directory 
	7.rm filename:Remove file 
	8.cp filename:Copy file
	9.load filename:upload file
	>>>q or Q to close ftp client,help to show order 

plan:
	add laod/cp directory 
	fix user interface more easy to use
