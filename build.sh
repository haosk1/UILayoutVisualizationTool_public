rm -rf ./output
rm -rf ./build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8


mkdir -p ../output/frame/UI
cp ./haosk_project ../output/
cp ../library/baselib/libbaselib.so ../output/
cp ../library/frame/libframe.so ../output/
# cp  -r ../library/JeMalloc ../output/
cp -r ../UIWork/res  ../output/frame/UI

cp -r ../UserInfo/*  ../output/

cd ..
rm -rf ./build




# rm ./Mutex.o
# rm ./ThreadPool.o
# rm ./TimerManager.o
# rm ./Timer.o
# rm ./main.o
# rm ./a.out

# g++ -std=c++11 -c -g ./baselib/Mutex/Mutex.cpp
# g++ -std=c++11 -c -g ./baselib/Timer/TimerManager.cpp
# g++ -std=c++11 -c -g ./baselib/Timer/Timer.cpp
# g++ -std=c++11 -c -g ./main/main.cpp
# g++ main.o Mutex.o TimerManager.o Timer.o -o a.out -lpthread

# rm ./1.txt
# # ./a.out >> ./1.txt
cd ./output
export LD_LIBRARY_PATH=./
# export LD_PRELOAD="./JeMalloc/libjemalloc.so.2"
# export MALLOC_CONF="prof:true,lg_prof_interval:25,lg_prof_sample:25,prof_prefix:./haosk_project"
 ./haosk_project
