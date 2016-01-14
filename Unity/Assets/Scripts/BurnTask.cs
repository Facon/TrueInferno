using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Assets.Scripts
{
    public class BurnTask : SoulTask
    {
        void SoulTask.executeTask(Building building)
        {
            building.SendMessage("soulReadyToBurn", SendMessageOptions.RequireReceiver);
        }
    }
}
