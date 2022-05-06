using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class delay25 : MonoBehaviour
{
    // Start is called before the first frame update

    public GameObject current;
    public GameObject UI;
    void OnEnable()
    {
        StartCoroutine(ExampleCoroutine());
    }

    IEnumerator ExampleCoroutine()
    {
        //Print the time of when the function is first called.
        Debug.Log("Started Coroutine at timestamp : " + Time.time);

        //yield on a new YieldInstruction that waits for 5 seconds.
        yield return new WaitForSeconds(23);

        //After we have waited 5 seconds print the time again.
        Debug.Log("Finished Coroutine at timestamp : " + Time.time);
        current.SetActive(false);
        UI.GetComponent<handData>().nextGesture();
    }
}
