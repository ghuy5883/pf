using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SnapToCenter : MonoBehaviour
{
    // Start is called before the first frame update
    //public GameObject player;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnCollisionEnter(Collision collision)
    {
        Debug.Log("snapping");
        if (collision.gameObject.tag == "Player")
        {
            collision.gameObject.transform.position = new Vector3(collision.gameObject.transform.position.x, collision.gameObject.transform.position.y, transform.position.z);
        }
    }
}
