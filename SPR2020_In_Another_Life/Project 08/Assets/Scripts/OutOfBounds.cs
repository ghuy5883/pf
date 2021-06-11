using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OutOfBounds : MonoBehaviour
{
     void OnBecameInvisible() {
         Destroy(gameObject);
     }
}
