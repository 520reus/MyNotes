#include <iostream>
#include <ros/ros.h>
#include <algorithm>
#include <vector>


using std::cout; using std::endl;

template<typename T> class DynamicArray
{
    public:
        void Insert(T num)
        {
            //为保持数据平均分配到两个堆中，在数据总数是偶数时把新数据插入最小堆，否则插入最大堆
            if(((max.size() + min.size()) & 1) == 0) //偶数，新数据插入最小堆
            {
                //如果新插入的数据比最大堆的数据中还小，将最大堆的堆顶最大元素插入最小堆
                if(max.size() > 0 && num < max[0])
                {
                    max.push_back(num);
                    std::push_heap(max.begin(),max.end(),std::less<T>());
                    num = max[0];
                    std::pop_heap(max.begin(),max.end(),std::less<T>());
                    max.pop_back();
                }

                min.push_back(num);
                std::push_heap(min.begin(),min.end(),std::greater<T>());
            }
            else
            {
                if(min.size() > 0 && num > min[0])
                {
                    min.push_back(num);
                    std::push_heap(min.begin(),min.end(),std::greater<T>());
                    num = min[0];
                    std::pop_heap(min.begin(),min.end(),std::greater<T>());
                    min.pop_back();
                }

                max.push_back(num);
                std::push_heap(max.begin(),max.end(),std::less<T>());
            }
        }

        T GetMedian()
        {
            int size = max.size() + min.size();
            if(size == 0)
                return -1;

            if((size & 1) == 0)
                return (max[0] + min[0])/2;
            
            else
                return min[0];
        }

    private:
        std::vector<T> max; //最大堆在数组左边
        std::vector<T> min; //最小堆在数组右边
};


int main(int argc, char** argv)
{
    DynamicArray<double> da;
    da.Insert(5.0);
    cout << da.GetMedian() << endl;
    da.Insert(4.0);
    cout << da.GetMedian() << endl;
    da.Insert(6.0);
    cout << da.GetMedian() << endl;
    return 0;
}