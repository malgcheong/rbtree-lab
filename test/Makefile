.PHONY: test

# c 컴파일러 옵션
CFLAGS=-I ../src -Wall -g #-DSENTINEL

# 생성하고자 하는 실행 파일 이름
Target = test

Target_src = rbtree
# Target_src = bst

Target_test = test-rbtree
# Target_test = test-bst


$(Target): $(Target_test)
	./$(Target_test)
	valgrind ./$(Target_test)

$(Target_test): $(Target_test).o ../src/$(Target_src).o

../src/$(Target_src).o:
	$(MAKE) -C ../src $(Target_src).o

clean:
	rm -f $(Target_test) *.o
