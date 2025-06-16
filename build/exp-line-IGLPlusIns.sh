# for delta in 100 200 300 400 500 600 700 800 900 1000
for delta in 500 1000 1500 2000 2500 3000
do
    echo ./IGLPlusIns $delta ../../Data/Dataset/DeltaG/$1.$delta.ge.txt ../../Data/Dataset/DeltaG/$1.$delta.in.txt ../../Data/exp/IGLPlusIns/$1.$delta.txt
    ./IGLPlusIns $delta ../../Data/Dataset/DeltaG/$1.$delta.ge.txt ../../Data/Dataset/DeltaG/$1.$delta.in.txt ../../Data/exp/IGLPlusIns/$1.$delta.txt
done