using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets.Scripts
{
    class PriorityQueue<T>
    {
        private SortedList<float,List<T>> data;

        public PriorityQueue()
        {
            data = new SortedList<float,List<T>>();
        }

        public void Add(T e, float score)
        {
            // Get the appropriate score list
            List<T> scoreList = data[score];
            
            // If it doesn't exist
            if (scoreList == null)
            {
                scoreList = new List<T>();
                data[score] = scoreList;
            }

            // Add element to the score list
            scoreList.Add(e);
        }

        public T popMin()
        {
            // Get the [score,list] pair with the minimum score
            KeyValuePair<float, List<T>> e = data.Min();

            // Get the first element of the list
            List<T> scoreList = e.Value;
            T ret = scoreList.First();

            // Remove element from list
            scoreList.Remove(ret);

            // If it was the last one
            if (scoreList.Count() == 0)
                // Delete score list
                data.Remove(e.Key);

            return ret;
        }
    }
}
