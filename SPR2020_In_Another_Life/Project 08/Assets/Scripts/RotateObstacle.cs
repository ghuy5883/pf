using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotateObstacle : MonoBehaviour
{
    // Start is called before the first frame update
    public float speed = 30.0f;
    public Vector3 axis;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        transform.RotateAround(Vector3.up, speed * Time.deltaTime);
        
    }
}
