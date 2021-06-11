using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    /* --------------------------------------------------------------
        CAMERA VARIABLES
    --------------------------------------------------------------- */
    [SerializeField]
    private Transform target;

    [SerializeField]
    private float smoothTime = 0.3f;

    private Vector3 cameraVelocity = Vector3.zero;

    /* --------------------------------------------------------------
        INIT VARIABLES
    --------------------------------------------------------------- */
    void Start()
    {
        if (target == null)
        {
            target = GameObject.FindGameObjectWithTag("Player").transform;
        }

    }

    /* --------------------------------------------------------------
        UPDATE VARIABLES
    --------------------------------------------------------------- */
    void Update()
    {
        Follow();

    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
    void Follow()
    {
       if (target) {
           // Define a target position above and behind the target transform
            Vector3 targetPosition = target.TransformPoint(new Vector3(0, 0, -10));

            // Smoothly move the camera towards that target position
            transform.position = Vector3.SmoothDamp(transform.position, targetPosition, ref cameraVelocity, smoothTime);
            //transform.position = Vector3.SmoothDamp(transform.position, targetPosition, ref cameraVelocity, smoothTime);
        }
    }
}
