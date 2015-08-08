using System;
using System.Collections.Generic;
using System.Linq;

namespace stream_sieve1
{
    class Program
    {
        static IEnumerable<int> rangefrom(int n)
        {
	        for(;;n++) yield return n;
        }

        static IEnumerable<int> sieve(IEnumerable<int> s)
        {
	        int cur = s.First();
            yield return cur;
            foreach(var item in sieve(s.Skip(1).Where(i=>i%cur != 0)))
            {
                yield return item;
            }
        }

        static void Main(string[] args)
        {   
            sieve(rangefrom(2)).TakeWhile(i=>i<100).ToList().ForEach(n => Console.WriteLine(n));
        }
    }
}
