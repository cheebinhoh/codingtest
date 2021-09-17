# old good makefile to help compiling
# 
# TODO: move into automake, libtool and autoconf for more scalability and proper dependency.

all : btreepathsum.out btreebltraverse.out btreemaxminlevel.out btreesymmetriccheck.out btreetraverse.out \
	btreebuild.out btreelca.out tree2btreebuild.out search-sort.out remove-c-comment.out \
 	string-utility.out calculator.out shrink-space.out trim-space.out replace-tab-with-space.out \
	syntax-validate-parenthese.out fold-line.out find2ndMaxNumber.out transformStr2Str.out \
	trafficdemand.out coding-test.out btreemirrorswap.out btreeidentical.out \
        btreerebalancing.out coding-test-2.out libbtree.a libllist.a list-test.out btreemaxpathsum.out \
	btreeisbalanced.out btreemaxnodeinlevel.out

libbtree.a : btree-internal.h btree.h btree.c avlbstree.h avlbstree.c llist.c llist.h llist-internal.h
	gcc -c btree.c avlbstree.c
	gcc -c llist.c 
	ar -rc libbtree.a btree.o avlbstree.o llist.o

libllist.a : llist.c llist.h llist-internal.h
	gcc -c llist.c
	ar -rc libllist.a llist.o

btreeisbalanced.out : btreeisbalanced.c btree.h libbtree.a
	gcc -o $@ btreeisbalanced.c -L. -lbtree

btreepathsum.out : btreepathsum.c btree.h libbtree.a
	gcc -o $@ btreepathsum.c -L. -lbtree

btreemaxpathsum.out : btreemaxpathsum.c btree.h libbtree.a
	gcc -o $@ btreemaxpathsum.c -L. -lbtree

btreebltraverse.out : btreebltraverse.c btree.h libbtree.a 
	gcc -o $@ btreebltraverse.c -L. -lbtree

btreemaxminlevel.out : btreemaxminlevel.c btree.h libbtree.a
	gcc -o $@ btreemaxminlevel.c -L. -lbtree

btreemaxnodeinlevel.out : btreemaxnodeinlevel.c btree.h libbtree.a
	gcc -o $@ btreemaxnodeinlevel.c -L. -lbtree

btreesymmetriccheck.out : btreesymmetriccheck.c btree.h libbtree.a
	gcc -o $@ btreesymmetriccheck.c -L. -lbtree

btreetraverse.out : btreetraverse.c btreetraverse-main.c btree.h btreetraverse.h libbtree.a
	gcc -o $@ btreetraverse.c btreetraverse-main.c -L. -lbtree

btreebuild.out : btreebuild.c btreetraverse.c btree.h btreetraverse.h libbtree.a
	gcc -o $@ btreebuild.c btreetraverse.c -L. -lbtree 

btreelca.out : btreelca.c btreetraverse.c btree.h btreetraverse.h libbtree.a
	gcc -o $@ btreelca.c btreetraverse.c -L. -lbtree

tree2btreebuild.out : tree2btreebuild.c btreetraverse.c btree.h btreetraverse.h libbtree.a
	gcc -o $@ tree2btreebuild.c btreetraverse.c -L. -lbtree

search-sort.out : search-sort.c 
	gcc -o $@ search-sort.c 

remove-c-comment.out : remove-c-comment.c
	gcc -o $@ remove-c-comment.c

string-utility.out : string-utility.c
	gcc -o $@ string-utility.c

calculator.out : calculator.c
	gcc -o $@ calculator.c

shrink-space.out : shrink-space.c
	gcc -o $@ shrink-space.c

trim-space.out : trim-space.c
	gcc -o $@ trim-space.c 

replace-tab-with-space.out : replace-tab-with-space.c 
	gcc -o $@ replace-tab-with-space.c 

syntax-validate-parenthese.out : syntax-validate-parenthese.c
	gcc -o $@ syntax-validate-parenthese.c

fold-line.out : fold-line.c
	gcc -o $@ fold-line.c

find2ndMaxNumber.out : find2ndMaxNumber.c
	gcc -o $@ find2ndMaxNumber.c

transformStr2Str.out : transformStr2Str.c
	gcc -o $@ transformStr2Str.c

trafficdemand.out : trafficdemand.c
	gcc -o $@ trafficdemand.c

coding-test.out : coding-test.c btree.h libbtree.a
	gcc -o $@ coding-test.c -L. -lbtree

btreemirrorswap.out : btreemirrorswap.c btree.h libbtree.a
	gcc -o $@ btreemirrorswap.c -L. -lbtree

btreeidentical.out : btreeidentical.c btree.h libbtree.a
	gcc -o $@ btreeidentical.c -L. -lbtree

btreerebalancing.out : btreerebalancing.c libbtree.a btree.h
	gcc -o $@ btreerebalancing.c -L. -lbtree

coding-test-2.out : coding-test-2.c libbtree.a btree.h libllist.a llist.h
	gcc -o $@ coding-test-2.c -L. -lbtree -lllist

list-test.out : list-test.c libllist.a
	gcc -o $@ list-test.c -L. -lllist

clean:
	rm -f *.out *.o lib*.a


test: 
	make all
	@echo
	./transformStr2Str.out -d | diff - ./transformStr2Str_result.txt
	./coding-test.out | diff - ./coding-test_result.txt 
