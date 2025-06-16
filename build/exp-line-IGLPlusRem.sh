# for delta in 100 200 300 400 500 600 700 800 900 1000
for delta in 500 1000 1500 2000 2500 3000
do
    echo ./IGLPlusRem $delta ../../Data/Dataset/$1.txt ../../Data/Dataset/DeltaG/$1.$delta.in.txt ../../Data/exp/IGLPlusRem/$1.$delta.txt
    ./IGLPlusRem $delta ../../Data/Dataset/$1.txt ../../Data/Dataset/DeltaG/$1.$delta.in.txt ../../Data/exp/IGLPlusRem/$1.$delta.txt
done