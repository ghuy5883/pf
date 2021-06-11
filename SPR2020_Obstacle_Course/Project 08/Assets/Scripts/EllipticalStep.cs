using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EllipticalStep : MonoBehaviour
{
    /* --------------------------------------------------------------
        COLLISION DETECTION
    --------------------------------------------------------------- */

    void OnCollisionStay(Collision other){
        if(other.gameObject.tag == "Player"){
            // Implements "moving platform" behavior
            other.transform.parent = transform;
 
        }
     }
 
    void OnCollisionExit(Collision other){
     if(other.gameObject.tag == "Player"){
            other.transform.parent = null;
             
         }
     }
}
