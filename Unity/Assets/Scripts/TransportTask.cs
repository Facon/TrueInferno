using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Assets.Scripts
{
    public class TransportTask : SoulTask
    {
        private float numResources;
        private ResourceType resourceType;
        private Building targetBuilding;

        public TransportTask(float numResources, ResourceType resourceType, Building targetBuilding)
        {
            this.numResources = numResources;
            this.resourceType = resourceType;
            this.targetBuilding = targetBuilding;
        }

        public float getNumResources()
        {
            return numResources;
        }

        public ResourceType getResourceType()
        {
            return resourceType;
        }

        public Building getTargetBuilding()
        {
            return targetBuilding;
        }

        void SoulTask.executeTask(Building building)
        {
            building.SendMessage("addInputResources", numResources, SendMessageOptions.RequireReceiver);
        }
    }
}
