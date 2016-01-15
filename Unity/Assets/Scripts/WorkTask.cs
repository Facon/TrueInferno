using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Assets.Scripts
{
    public class WorkTask : SoulTask
    {
        void SoulTask.executeTask(Building building)
        {
            // Notify building the soul has just reached and it's ready to work
            building.SendMessage("soulReadyToWork", SendMessageOptions.RequireReceiver);
        }
    }
}
