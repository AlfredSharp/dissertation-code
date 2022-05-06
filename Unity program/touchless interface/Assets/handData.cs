using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;
using System.IO;
using System;
using System.Diagnostics;
using Ultraleap.TouchFree.Tooling;
using Ultraleap.TouchFree.Tooling.Connection;

using Ultraleap;
public class handData : MonoBehaviour
{
    public GameObject partNo;
    public GameObject gestureOrder;
    public GameObject distOrder;

    public GameObject zoomGO;
    public GameObject horizontalGO;
    public GameObject verticalGO;
    public GameObject farGO;
    public GameObject medGO;
    public GameObject closeGO;

    public int currentGesture = 0;
    private int currentDist = 0;
    public string gestureOrderInt;
    private string distOrderInt;
    public IDictionary<char, GameObject> gestureDict = new Dictionary<char, GameObject>();
    private IDictionary<char, GameObject> distanceDict = new Dictionary<char, GameObject>();


    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void begin(){
        //output to console
        UnityEngine.Debug.Log(gestureOrder.GetComponent<Text>().text);

        gestureOrderInt = gestureOrder.GetComponent<Text>().text;
        distOrderInt = distOrder.GetComponent<Text>().text;

        gestureDict.Add('0', zoomGO);
        gestureDict.Add('1', horizontalGO);
        gestureDict.Add('2', verticalGO);

        distanceDict.Add('0', farGO);
        distanceDict.Add('1', medGO);
        distanceDict.Add('2', closeGO);
        
        // distanceDict[distOrderInt[currentDist]].SetActive(true);
        // distanceDict['1'].SetActive(true);

        // currentDist++;
        nextDistance();
    }

    public void nextGesture(){
        UnityEngine.Debug.Log(gestureOrder.GetComponent<Text>().text[0]);
        if(currentGesture < 3){
            if(gestureOrder.GetComponent<Text>().text[currentGesture] == '0'){
                zoomGO.SetActive(true);
            }else if(gestureOrder.GetComponent<Text>().text[currentGesture] == '1'){
                horizontalGO.SetActive(true);
            }else if(gestureOrder.GetComponent<Text>().text[currentGesture] == '2'){
                verticalGO.SetActive(true);
            }
            currentGesture++;
        }
        else{
            currentGesture = 0;
            nextDistance();
        }
    }

    public void nextDistance(){

        if(currentDist < 3){
            if(distOrder.GetComponent<Text>().text[currentDist] == '0'){
                farGO.SetActive(true);
            }else if(distOrder.GetComponent<Text>().text[currentDist] == '1'){
                medGO.SetActive(true);
            }else if(distOrder.GetComponent<Text>().text[currentDist] == '2'){
                closeGO.SetActive(true);
            }
            currentDist++;
        }
        // else{
        //     currentDist = 0;
        //     nextDistance();
        // }
    }

    public void recordToCsv(string gesture){
        string[] data = new string[3];
        data[0] = partNo.GetComponent<Text>().text;
        data[1] = System.DateTime.Now.ToString("HH:mm:ss:ffff");
        data[2] = gesture;
        
        //write data to csv file
        string path = Application.dataPath + "/Resources/data.csv";
        File.AppendAllLines(path, new string[] { string.Join(",", data) });

        Process process = new Process();
        process.StartInfo.FileName = Environment.CurrentDirectory + @"\recordToFile.exe";
        string fileName = "participant" + partNo.GetComponent<Text>().text + "recordingGesture" + gesture + "distance"+ currentDist + ".lmt";
        process.StartInfo.Arguments = fileName;
        process.StartInfo.UseShellExecute = false;
        process.StartInfo.CreateNoWindow = true;
        process.Start();
    }

    private string getPath(){
        #if UNITY_EDITOR
        return Application.dataPath +"/CSV/"+"Saved_data.csv";
        #elif UNITY_ANDROID
        return Application.persistentDataPath+"Saved_data.csv";
        #elif UNITY_IPHONE
        return Application.persistentDataPath+"/"+"Saved_data.csv";
        #else
        return Application.dataPath +"/"+"Saved_data.csv";
        #endif
    }
}
