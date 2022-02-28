#include <ros/ros.h>
#include <iostream>

using std::cout; using std::endl;

void replaceSpace(char* string, int length)
{
    if(string == nullptr || length <= 0)
        return;
    
    int blank_num = 0;
    int origin_length = 0;
    int i = 0;
    while(string[i] != '\0')
    {
        origin_length++;
        if(string[i] == ' ')
            blank_num++;
        i++;
    }

    int new_length = origin_length + blank_num*2;
    int index_of_origin = origin_length;
    int index_of_new = new_length;
    while(index_of_origin >= 0 && index_of_new > index_of_origin)
    {
         if(string[index_of_origin] == ' ')
         {
             string[index_of_new--] = '0';
             string[index_of_new--] = '2';
             string[index_of_new--] = '%';
         }
         else
         {
             string[index_of_new--] = string[index_of_origin];
         }

         index_of_origin--;
    }
}

int main(int argc, char* argv)
{
    char string[30] = "We are happy.";
    replaceSpace(string, 30);
    cout << string << endl;
    return 0;
}