using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaunchPad : MonoBehaviour
{
    /* --------------------------------------------------------------
        LAUNCHPAD VARIABLES
    --------------------------------------------------------------- */
    public float normalBounceStrength;
    public float sprintBounceStrength;
    public float bounceStrength;

    /* --------------------------------------------------------------
        INSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    void Start()
    {
        
    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    void Update()
    {
        
    }

    /* --------------------------------------------------------------
        COLLISION DETECTION
    --------------------------------------------------------------- */
    void OnTriggerStay(Collider other) {
        if(other.tag == "Player") {

            // Reduce downward velocity and boost upwards
            if(other.gameObject.GetComponent<Rigidbody>().velocity.y < 0.0f) {
                Vector3 temp = other.gameObject.GetComponent<Rigidbody>().velocity;
                temp.y *= 0.5f;
                other.gameObject.GetComponent<Rigidbody>().velocity = temp;
            }
            
            other.gameObject.GetComponent<Rigidbody>().AddForce(Vector3.up * bounceStrength, ForceMode.Impulse);
        }
    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
}
