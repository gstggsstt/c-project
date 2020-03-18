g++ ./Sort.cpp -o ./Sort
g++ ./My_sort.c -o ./My_sort
g++ ./Quick_sort.c -o ./Quick_sort
g++ ./Heap_sort.c -o ./Heap_sort
g++ ./Merge_sort.c -o ./Merge_sort
while echo 100000 >in && ./genn 30000 100000 200000 -1 >>in && ./genn 15000 50000 100000 -1 >>in && ./genn 55000 1 50000 -1 >>in && time ./Sort <in >out1 && time ./My_sort <in >out2 && time ./Quick_sort <in >out && time ./Heap_sort <in >out && time ./Merge_sort <in >out && diff -w ./out1 ./out2
do
	echo Accepted
done
