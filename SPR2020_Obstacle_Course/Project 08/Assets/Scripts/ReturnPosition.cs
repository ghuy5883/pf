using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ReturnPosition : MonoBehaviour
{
    /* --------------------------------------------------------------
        POSITON VARIABLES
    --------------------------------------------------------------- */
    public Vector3 startingPosition;

    /* --------------------------------------------------------------
        iNSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    void Start() {
        startingPosition = gameObject.transform.position;
    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
    public void ResetPosition() {
        gameObject.GetComponent<Rigidbody>().velocity = new Vector3(0f,0f,0f);
        gameObject.GetComponent<Rigidbody>().angularVelocity = new Vector3(0f,0f,0f);
        gameObject.GetComponent<Rigidbody>().rotation = Quaternion.Euler(new Vector3(0f,0f,0f));
        gameObject.transform.position = startingPosition;
    }

}
