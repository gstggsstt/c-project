rm -rf ./res

echo TEST1 Start!
echo 3 >in
./gen 3 1 10 -1 >>in
echo Test1: [3,1,10] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST1: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10 )>>err
echo TEST1 Done!

echo TEST2 Start!
echo 7 >in
./gen 7 1 100 -1 >>in
echo Test2: [7,1,100] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST2: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST2 Done!

echo TEST3 Start!
echo 8 >in
./gen 8 1 100 -1 >>in
echo Test3: [8,1,100] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST3: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST3 Done!

echo TEST4 Start!
echo 10 >in
./gen 10 1 100 -1 >>in
echo Test4: [10,1,100] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST4: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST4 Done!

echo TEST5 Start!
echo 50 >in
./gen 50 1 100 -1 >>in
echo Test5: [50,1,100] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST5: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST5 Done!

echo TEST6 Start!
echo 100 >in
./gen 100 1 100 -1 >>in
echo Test6: [100,1,100] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST6: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST6 Done!

echo TEST7 Start!
echo 1000 >in
./gen 1000 1 10000 -1 >>in
echo Test7: [1000,1,10000] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST7: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST7 Done!

echo TEST8 Start!
echo 10000 >in
./gen 10000 1 10000 -1 >>in
echo Test8: [10000,1,10000] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST8: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST8 Done!

echo TEST9 Start!
echo 100000 >in
./gen 100000 1 100000 -1 >>in
echo Test9: [100000,1,100000] >>res
#./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
#./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
#./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST9: >>err
echo TEST10: >>err
(diff -w ./out2 ./out3 &&
diff -w ./out3 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out8 &&
diff -w ./out8 ./out9 && diff -w ./out9 ./out10) >>err
echo TEST9 Done!

echo TEST10 Start!
echo 1000000 >in
./gen 1000000 1 1000000 -1 >>in
echo Test10: [1000000,1,1000000] >>res
#./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
#./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
#./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST10: >>err
(diff -w ./out2 ./out3 &&
diff -w ./out3 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out8 &&
diff -w ./out8 ./out9 && diff -w ./out9 ./out10) >>err
echo TEST10 Done!

echo TEST11 Start!
echo 10000 >in
./gen 10000 1 1 -1 >>in
echo Test11: [10000,1,1] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST11: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST11 Done!

echo TEST12 Start!
echo 10000 >in
./gen 10000 1 2 -1 >>in
echo Test12: [10000,1,2] >>res
./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST12: >>err
(diff -w ./out1 ./out2 && diff -w ./out2 ./out3 && diff -w ./out3 ./out4 &&
diff -w ./out4 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out7 &&
diff -w ./out7 ./out8 && diff -w ./out8 ./out9 && diff -w ./out9 ./out10)>>err
echo TEST12 Done!

echo TEST13 Start!
echo 1000000 >in
./gen 100000 100000 1100000 -1 >>in
./gen 100000 90000  1000000 -1 >>in
./gen 100000 80000  900000 -1 >>in
./gen 100000 70000  800000 -1 >>in
./gen 100000 60000  700000 -1 >>in
./gen 100000 50000  600000 -1 >>in
./gen 100000 40000  500000 -1 >>in
./gen 100000 30000  400000 -1 >>in
./gen 100000 20000  300000 -1 >>in
./gen 100000 10000  200000 -1 >>in
echo Test13:[100000,1000000,1100000]+ [100000,900000,1000000]+ [100000,800000,900000 ]+ [100000,700000,800000 ]+ [100000,600000,700000 ]+ [100000,500000,600000 ]+ [100000,400000,500000 ]+ [100000,300000,400000 ]+ [100000,200000,300000 ]+ [100000,100000,200000 ] >>res
#./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
#./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
#./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST13: >>err
(diff -w ./out2 ./out3 &&
diff -w ./out3 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out8 &&
diff -w ./out8 ./out9 && diff -w ./out9 ./out10) >>err
echo TEST13 Done!

echo TEST14 Start!
echo 1000000 >in
./gen 100000 10000  200000 -1 >>in
./gen 100000 20000  300000 -1 >>in
./gen 100000 30000  400000 -1 >>in
./gen 100000 40000  500000 -1 >>in
./gen 100000 50000  600000 -1 >>in
./gen 100000 60000  700000 -1 >>in
./gen 100000 70000  800000 -1 >>in
./gen 100000 80000  900000 -1 >>in
./gen 100000 90000  1000000 -1 >>in
./gen 100000 100000 1100000 -1 >>in
echo Test14: [100000,100000,200000 ]+ [100000,200000,300000 ]+ [100000,300000,400000 ]+ [100000,400000,500000 ]+ [100000,500000,600000 ]+ [100000,600000,700000 ]+ [100000,700000,800000 ]+ [100000,800000,900000 ]+ [100000,900000,1000000]+ [100000,1000000,1100000] >>res
#./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
#./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
#./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST14: >>err
(diff -w ./out2 ./out3 &&
diff -w ./out3 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out8 &&
diff -w ./out8 ./out9 && diff -w ./out9 ./out10) >>err
echo TEST14 Done!

echo TEST15 Start!
echo 1000000 >in
./gen 1000000 1 100 -1 >>in
echo Test15:[1000000,1,100] >>res
#./Bubble_sort <in >out1
./Counting_sort <in >out2
./Heap_sort <in >out3
#./Insertion_sort <in >out4
./Merge_sort <in >out5
./Quick_sort <in >out6
#./Selection_sort <in >out7
./Radix_sort <in >out8
./Sort <in >out9
./My_sort <in >out10
echo TEST15: >>err
(diff -w ./out2 ./out3 &&
diff -w ./out3 ./out5 && diff -w ./out5 ./out6 && diff -w ./out6 ./out8 &&
diff -w ./out8 ./out9 && diff -w ./out9 ./out10) >>err
echo TEST15 Done!

rm -rf ./out1 ./out2 ./out3 ./out4 ./out5 ./out6 ./out7 ./out8 ./out9 ./out10
