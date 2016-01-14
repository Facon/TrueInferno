using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets.Scripts
{
    public interface SoulTask
    {
        /// <summary>
        /// Execute task
        /// </summary>
        /// <param name="building"></param>
        void executeTask(Building building);
    }
}
