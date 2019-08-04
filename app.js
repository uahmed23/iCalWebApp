'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const mysql = require('mysql');
let connection = null;
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  try{

    uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });

}catch(err){
  res.send("no file to upload");
  }
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ********************

let sharedLib = ffi.Library('./libcal',{

  'objectToJSON': ['string', ['string']],
  'eventObjectToJSON': ['string', ['string']],
  'alarmObjectToJSON': ['string',['string','int']],
  'propObjectToJSON': ['string',['string','int']],
  'createCalendarWrapper': ['string',['string','string','string','string','string','string','string','string','string']],
  'addEventWrapper': ['string',['string','string','string','string','string','string','string','string']],
});

// let error;




app.get('/object',function(req , res){
  // let fileSelected = req.query.filename;
  let files = [];
  files = fs.readdirSync('./uploads/');
  let array = [];
  let tempStr;
  // var obj;

  //var j =0;
  for (var i =0; i < files.length; i++)
  {
    // obj = JSON.parse(array[i]);//
    array[i] = sharedLib.objectToJSON(files[i]);
  }
  res.send(array);
});

app.get('/events',function(req , res){
  let files = [];
  files = fs.readdirSync('./uploads/');
  let array = [];
  let tempStr;
  // var obj;

  //var j =0;
  for (var i =0; i < files.length; i++)
  {
    // obj = JSON.parse(array[i]);//

    array[i] = sharedLib.eventObjectToJSON(files[i]);
    // if(tempStr != "{}")
    // {
      //array[i] = tempStr;
      // console.log(array[i]);
      // j++;
    // }
  }
  res.send(array);
});

app.get('/alarms',function(req , res){
  let fileSelected = req.query.filename;
  let eventSelected = req.query.numEvent;
  // let files = [];
  // files = fs.readdirSync('./uploads/');
  // let array = [];
  let jsonStr;

    jsonStr = sharedLib.alarmObjectToJSON(fileSelected,eventSelected);

  res.send(jsonStr);
});


app.get('/properties',function(req , res){
  let fileSelected = req.query.filename;
  let eventSelected = req.query.numEvent;
  // let files = [];
  // files = fs.readdirSync('./uploads/');
  // let array = [];
  let jsonStr;

  // console.log(fileSelected);

  jsonStr = sharedLib.propObjectToJSON(fileSelected,eventSelected);

  // console.log("HLEOOO");
  res.send(jsonStr);
});

app.get('/createCal',function(req , res){
  let filename = req.query.filename;
  let calJSON = req.query.calJSON;
  let eventJSON= req.query.eventJSON;
  let eventsummary= req.query.eventsummary;
  let eventSD = req.query.eventSD;
  let eventST = req.query.eventST;
  let eventUTC= req.query.eventUTC;
  let eventCD = req.query.eventCD;
  let eventCT = req.query.eventCT;

  let jsonStr;
  //
  jsonStr = sharedLib.createCalendarWrapper(filename,calJSON,eventJSON,eventsummary,eventSD,eventST,eventUTC,eventCD,eventCT);
// 'createCalendarWrapper': ['string',['string','string','string','string','string','string','string']],
  // console.log(jsonStr);

  res.send(jsonStr);
});

app.get('/addEvent',function(req , res){
  let filename = req.query.filename;
  let eventJSON= req.query.eventJSON;
  let eventsummary= req.query.eventsummary;
  let eventSD = req.query.eventSD;
  let eventST = req.query.eventST;
  let eventUTC= req.query.eventUTC;
  let eventCD = req.query.eventCD;
  let eventCT = req.query.eventCT;

  let jsonStr;

  jsonStr = sharedLib.addEventWrapper(filename,eventJSON,eventsummary,eventSD,eventST,eventUTC,eventCD,eventCT);

  // console.log(jsonStr);

  res.send(jsonStr);
});

app.get('/filenames', function(req , res){
  let files = [];
  files = fs.readdirSync('./uploads/');
  var myJSON = JSON.stringify(files);
  res.send(myJSON);
});



// ASSIGNMENT 4 FUNCTIONALLITY-------------------------------------------------------------------------------------------------------

// const connection;

app.get('/dbconnection',function(req , res){
  let username = req.query.username;
  let password= req.query.password;
  let dbname= req.query.dbname;
  let errorNum = 0;
  let error = "connected";


  connection = mysql.createConnection({
      host     : 'dursley.socs.uoguelph.ca',
      user     :  username,
      password :  password,
      database :  dbname
  });
  // res.send("99");
   // connection.connect();
  // errorNum = connection.connect(function(err) {

  connection.connect(function(err) {
    if (err && err.message!="ER_CON_COUNT_ERROR: Too many connections") {
      // connection.end();
      console.log('error: ' + err.message);
      res.send("{\"error\":\"fail\"}");
    }
    else if(err && err.message == "ER_CON_COUNT_ERROR: Too many connections")
    {
      // connection.end();
      console.log('error: ' + err.message);
      res.send("{\"error\":\"fail\"}");
    }
    else{
      console.log('Connected to the MySQL server.');
      connection.query("create table FILE (cal_id int not null auto_increment primary key,  file_Name varchar(60) not null,  version int not null, prod_id varchar(256) not null )", function (err, rows, fields) {
          if (err) console.log("Something went wrong. "+err);
      });
      connection.query("create table EVENT (event_id int not null auto_increment primary key,  summary varchar(1024), start_time datetime not null, location varchar(60), organizer varchar(256), cal_file int not null, FOREIGN KEY(cal_file) REFERENCES FILE(cal_id) ON DELETE CASCADE )", function (err, rows, fields) {
          if (err) console.log("Something went wrong. "+err);
      });
      connection.query("create table ALARM (alarm_id int not null auto_increment primary key,  action varchar(256) not null, `trigger` varchar(256) not null, event int not null, FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE )", function (err, rows, fields) {
          if (err) console.log("Something went wrong. "+err);
      });
      // error = "isdjfijdf";
      // return;
      // connection.end();
      res.send("{\"error\":\"success\"}");

      // res.send("0");
    }

  });

  let jsonStr = "{\"error\":\""+error+"\"}";

});

//CLEAR ALL DATA------------------------------------------------------------------------------------------------------------------------
app.get('/cleardb',function(req , res){
  let username = req.query.username;
  let password= req.query.password;
  let dbname= req.query.dbname;
  let error = "connected";

  connection.query("delete from FILE", function (err, rows, fields) {
      if (err)
          console.log("Something went wrong. "+err);
      else
          console.log("Cleared data from table");
  });

  let jsonStr = "{\"error\":\""+error+"\"}";
  res.send(jsonStr);
});

//STORE ALL FILES----------------------------------------------------------------------------------------------------------------------
app.get('/storedb',function(req , res){

  let filenames = req.query.filenames;
  let i = 0;
  let fileArray = [];
  let jsonTemp;
  let records = [];
  let array = [];
  // let goOn = false;


  connection.query("delete from FILE", function (err, rows, fields) {
      if (err)
          console.log("Something went wrong. "+err);
      else{
          for(i=0;i<filenames.length;i++)
          {
            fileArray[i] = sharedLib.objectToJSON(filenames[i]);
            // events = sharedLib.eventObjectToJSON(filenames[i]);
            // console.log(events);

            jsonTemp = JSON.parse(fileArray[i]);
            records[i] =  "INSERT INTO FILE (file_Name, version, prod_id) VALUES ('"+jsonTemp.filename+"','"+jsonTemp.version+"','"+jsonTemp.prodID+"')";
            connection.query(records[i], function (err, rows, fields) {
                if (err)
                    console.log("Something went wrong. "+err);
                else{
                }
            });
          }

        }//end of delete else
  });//end of delete query

  let jsonStrReturn = "{\"error\":\"success\"}";
  res.send(jsonStrReturn);
});

//STORE EVENTS-------------------------------------------------------------------------------------------------------------------------
app.get('/storeEvents',function(req , res){

  // let filenames = req.query.filenames;
  let i = 0;
  // let fileArray = [];
  let events;
  // let jsonTemp;
  let records;
  let alarmRecord;
  let obj;
  let j;
  let summary = null;
  let location = null;
  let organizer = null;
  let propStr;
  let propObj;
  let alarmStr;
  let alarmObj;
  let datetimeStr;

  // let array = [];

  console.log("!!!!");


  connection.query("delete from EVENT", function (err, rows, fields) {
      if (err)
          console.log("Something went wrong. "+err);
      else{

        connection.query("SELECT * FROM FILE ORDER BY cal_id", function (err, rows, fields) {
          //Throw an error if we cannot run the query
          if (err)
          console.log("Something went wrong. "+err);
          else {

            // console.log("FILE contents sorted by cal_id:");
            for (let row of rows){

              events = sharedLib.eventObjectToJSON(row.file_Name);
              obj = JSON.parse(events);
              for (j in obj ) {
                if(j!=0)
                {
                  propStr = sharedLib.propObjectToJSON(row.file_Name,j);
                  // alarmStr = sharedLib.alarmObjectToJSON(row.file_Name,j);
                  // alarmObj = JSON.parse(alarmStr);
                  // console.log(alarmStr);

                  // for ( i in alarmObj){
                  //   console.log("Filename: "+row.file_Name+" Event no.: "+j+" Alarm no.: "+i+" :- "+alarmObj[i].action+" "+alarmObj[i].trigger+" "+alarmObj[i].numProps);
                  //   alarmRecord =  "INSERT INTO ALARM (action, trigger, event) VALUES ('"+alarmObj[i].action +"','"+alarmObj[i].trigger+"','"+row.cal_id+"')";
                  // }
                  // alarmRecord =  alarmObj[i].action+" "+alarmObj[i].trigger+" "+alarmObj[i].numProps;



                  propObj = JSON.parse(propStr);
                  summary = null;
                  location = null;
                  organizer = null;
                  for ( i in propObj){
                    if(propObj[i].propName == "SUMMARY"){
                      summary = propObj[i].propDescr;
                    }
                    if(propObj[i].propName == "ORGANIZER"){
                      organizer = propObj[i].propDescr;
                    }
                    if(propObj[i].propName == "LOCATION"){
                      location = propObj[i].propDescr;
                    }
                  }
                  datetimeStr = obj[j].startDT.date.slice(0,4)+"-"+obj[j].startDT.date.slice(4,6)+"-"+obj[j].startDT.date.slice(6,8)+" "+obj[j].startDT.time.slice(0,2)+":"+obj[j].startDT.time.slice(2,4)+":" +obj[j].startDT.time.slice(4,6);
                  // console.log("j = "+j+" "+row.file_Name);
                  records =  "INSERT INTO EVENT (summary, start_time, location, organizer, cal_file) VALUES ('"+summary+"','"+datetimeStr+"','"+location+"','"+organizer+"','"+row.cal_id+"')";

                  connection.query(records, function (err, rows, fields) {
                      if (err)
                          console.log("Something went wrong. "+err);
                      else{
                        // console.log("In else, j="+j+" "+alarmStr);
                        //
                        // for ( i in alarmObj){
                        //   console.log("Filename: "+row.file_Name+" Event no.: "+j+" Alarm no.: "+i+" :- "+alarmObj[i].action+" "+alarmObj[i].trigger+" "+alarmObj[i].numProps);
                        //   alarmRecord =  "INSERT INTO ALARM (action, trigger, event) VALUES ('"+alarmObj[i].action+"','"+alarmObj[i].trigger+"','"+row.cal_id+"')";
                        // }
                      }
                  });
                  // console.log(j+": "+ records);
                }//end of if
              }//end of obj of event for
              // console.log("ID: "+row.cal_id+" file_Name: "+row.file_Name+" version: "+row.version+" prod_id: "+row.prod_id );
            }//end of for
          }//end of else
        });//end of FILE query
        // connection.query("SELECT * FROM FILE ORDER BY cal_id", function (err, rows, fields) {
        //   //Throw an error if we cannot run the query
        //     if (err)
        //         console.log("Something went wrong. "+err);
        //     else {
        //         console.log("FILE contents sorted by cal_id:");
        //
        //         //Rows is an array of objects.  Each object has fields corresponding to table columns
        //         for (let row of rows){
        //             console.log("cal_id: "+row.cal_id+"  file_Name: "+row.file_Name+" version: "+row.version+" prod_id: "+row.prod_id );
        //         }
        //         connection.query("SELECT * FROM EVENT ORDER BY cal_file", function (err, rows, fields) {
        //           //Throw an error if we cannot run the query
        //             if (err)
        //                 console.log("Something went wrong. "+err);
        //             else {
        //                 console.log("EVENT contents sorted by cal_file:");
        //
        //                 //Rows is an array of objects.  Each object has fields corresponding to table columns
        //                 for (let row of rows){
        //                     console.log("event_id: "+row.event_id+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
        //                 }
        //             }
        //           });
        //     }
        //   });

        }//end of delete else
  });//end of delete query

  let jsonStrReturn = "{\"error\":\"success\"}";
  res.send(jsonStrReturn);
});

//STORE ALARMS-------------------------------------------------------------------------------------------------------------------------
app.get('/storeAlarms',function(req , res){

  // let filenames = req.query.filenames;
  let i = 0;
  // let fileArray = [];
  let events;
  // let jsonTemp;
  let records;
  let alarmRecord;
  let obj;
  let j;
  let summary = null;
  let location = null;
  let organizer = null;
  let propStr;
  let propObj;
  let alarmStr;
  let alarmObj;
  let datetimeStr;

  // let array = [];

  console.log("????");


  connection.query("delete from ALARM", function (err, rows, fields) {
      if (err)
          console.log("Something went wrong. "+err);
      else{

        connection.query("SELECT * FROM FILE ORDER BY cal_id", function (err, rows, fields) {

          if (err)
          console.log("Something went wrong. "+err);
          else {
            for (let row of rows){
              // console.log(row.file_Name);

              connection.query("SELECT * FROM EVENT WHERE cal_file ="+row.cal_id+" ORDER BY event_id", function (err, rows, fields) {

                if (err)
                console.log("Something went wrong. "+err);
                else {
                  i=1;
                  for (let rowE of rows){
                    alarmStr = sharedLib.alarmObjectToJSON(row.file_Name,i);
                    alarmObj = JSON.parse(alarmStr);
                    // console.log(alarmStr);

                    for ( j in alarmObj){
                      // records =  "INSERT INTO EVENT (summary, start_time, location, organizer, cal_file) VALUES ('"+summary+"','"+datetimeStr+"','"+location+"','"+organizer+"','"+row.cal_id+"')";

                      // console.log("Filename: "+row.file_Name+" Event no.: "+i+" Alarm no.: "+j+" :- "+alarmObj[j].action+" "+alarmObj[j].trigger+" "+alarmObj[j].numProps);
                      alarmRecord =  "INSERT INTO ALARM (action, `trigger`, event) VALUES ('"+alarmObj[j].action +"','"+alarmObj[j].trigger+"','"+rowE.event_id+"')";
                      connection.query(alarmRecord, function (err, rows, fields) {
                          if (err)
                              console.log("Something went wrong. "+err);
                          else{
                            // console.log("In else, j="+j+" "+alarmStr);
                            //
                            // for ( i in alarmObj){
                            //   console.log("Filename: "+row.file_Name+" Event no.: "+j+" Alarm no.: "+i+" :- "+alarmObj[i].action+" "+alarmObj[i].trigger+" "+alarmObj[i].numProps);
                            //   alarmRecord =  "INSERT INTO ALARM (action, trigger, event) VALUES ('"+alarmObj[i].action+"','"+alarmObj[i].trigger+"','"+row.cal_id+"')";
                            // }
                          }
                      });

                    }
                    // console.log(row.file_Name+" Event no. "+i+" "+rowE.start_time);



                    i++;
                  }//end of EVENT for
                }//end of EVENT else
              });//end of EVENT query


            }//end of FILE for
          }//end of FILE else
        });//end of FILE query
        // connection.query("SELECT * FROM FILE ORDER BY cal_id", function (err, rows, fields) {
        //   //Throw an error if we cannot run the query
        //     if (err)
        //         console.log("Something went wrong. "+err);
        //     else {
        //         console.log("FILE contents sorted by cal_id:");
        //
        //         //Rows is an array of objects.  Each object has fields corresponding to table columns
        //         for (let row of rows){
        //             console.log("cal_id: "+row.cal_id+"  file_Name: "+row.file_Name+" version: "+row.version+" prod_id: "+row.prod_id );
        //         }
        //         connection.query("SELECT * FROM EVENT ORDER BY cal_file", function (err, rows, fields) {
        //           //Throw an error if we cannot run the query
        //             if (err)
        //                 console.log("Something went wrong. "+err);
        //             else {
        //                 console.log("EVENT contents sorted by cal_file:");
        //
        //                 //Rows is an array of objects.  Each object has fields corresponding to table columns
        //                 for (let row of rows){
        //                     console.log("event_id: "+row.event_id+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
        //                 }
        //             }
        //           });
        //     }
        //   });

        }//end of delete else
  });//end of delete query

  let jsonStrReturn = "{\"error\":\"success\"}";
  res.send(jsonStrReturn);
});

//Sample endpoint
app.get('/someendpoint', function(req , res){
  // connection.end(function(err) {
  //   if (err) {
  //     // connection.end();
  //     console.log('error: ' + err.message);
  //   }
  //   else{
  //
  //   }
  //
  // });
  res.send({
    foo: "bar"
  });

  // connection.query("SELECT * FROM FILE ORDER BY cal_id", function (err, rows, fields) {
  //   //Throw an error if we cannot run the query
  //     if (err)
  //         console.log("Something went wrong. "+err);
  //     else {
  //         console.log("FILE contents sorted by cal_id:");
  //
  //         //Rows is an array of objects.  Each object has fields corresponding to table columns
  //         for (let row of rows){
  //             console.log("cal_id: "+row.cal_id+"  file_Name: "+row.file_Name+" version: "+row.version+" prod_id: "+row.prod_id );
  //         }
  //         connection.query("SELECT * FROM EVENT ORDER BY cal_file", function (err, rows, fields) {
  //           //Throw an error if we cannot run the query
  //             if (err)
  //                 console.log("Something went wrong. "+err);
  //             else {
  //                 console.log("EVENT contents sorted by cal_file:");
  //
  //                 //Rows is an array of objects.  Each object has fields corresponding to table columns
  //                 for (let row of rows){
  //                     console.log("event_id: "+row.event_id+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
  //                 }
  //                 connection.query("SELECT * FROM ALARM ORDER BY event", function (err, rows, fields) {
  //                   //Throw an error if we cannot run the query
  //                     if (err)
  //                         console.log("Something went wrong. "+err);
  //                     else {
  //                         console.log("alarm contents sorted by event:");
  //
  //                         //Rows is an array of objects.  Each object has fields corresponding to table columns
  //                         for (let row of rows){
  //                             console.log("alarm_id: "+row.alarm_id+"  action: "+row.action+" trigger: "+" event: "+row.event);
  //                         }
  //                     }
  //                   });
  //             }
  //           });
  //     }
    });

    //DISPLAY DB

    app.get('/displaydb',function(req , res){

      let files = 2 ;
      let events= 2;
      let alarms= 2;

      console.log("displaydb");

      // var sql = "SELECT count(*) as total FROM FILE";
      connection.query("SELECT count(*) as total FROM FILE", function(err, result) {

        console.log(result[0].total);
       // files  = result[0].total;

       connection.query("SELECT count(*) as total FROM EVENT", function(err, result1) {

         // events  = result[0].total;
         console.log(result1[0].total);

         connection.query("SELECT count(*) as total FROM ALARM", function(err, result2) {

           // alarms  = result[0].total;

           console.log(result2[0].total);
           // res.send("{\"error\":\"success\"}");

           // console.log("------"+result[0].total+result1[0].total+result2[0].total);
           res.send("{\"error\":\"success\",\"files\":\""+result[0].total+"\",\"events\":\""+result1[0].total+"\",\"alarms\":\""+result2[0].total+"\"}");

         });

       });
      });
    });

//Q1------------------------------------------------------------------------------------------------------
    app.get('/q1',function(req , res){

        let array = [];
        let i = 0;
        let j =0;

        connection.query("SELECT * FROM EVENT ORDER BY start_time", function (err, rows, fields) {
          if (err)
              console.log("Something went wrong. "+err);
          else {
              // console.log("EVENT contents sorted by startdat:");
                  i= 0;
                  j=1;
                  for (let row of rows){
                    array[i] = "Event#: "+j+"  Summary: "+row.summary+" Start date/time: "+row.start_time+" Location: "+row.location+" Organizer: "+row.organizer+" file_Name: "+row.cal_file;
                    // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
                    // console.log(array[i]);
                    i++;
                    j++;
                }
                let jsonstr = JSON.stringify(array);
                // console.log(jsonstr);
                res.send(jsonstr);


          }
      });


    });

    app.get('/q2',function(req , res){

      let filename = req.query.filename;

        let array = [];
        let i = 0;
        let j =0;

        connection.query("SELECT * FROM FILE WHERE file_Name=\'"+filename+"\'", function (err, rows, fields) {
          if (err)
              console.log("Something went wrong. "+err);
          else {
              console.log("FILE content:");
                  // i= 0;
                  // j=1;
                  if(rows.length == 0)
                  {

                    let jsonstr = JSON.stringify(array);
                    console.log(jsonstr);
                    res.send(jsonstr);

                  }
                  else{

                  for (let row of rows){
                    console.log(row.file_Name);

                    connection.query("SELECT * FROM EVENT WHERE cal_file=\'"+row.cal_id+"\'", function (err, rows, fields) {
                      if (err)
                      console.log("Something went wrong. "+err);
                      else {
                        console.log("EVENT content:");
                        i= 0;
                        j=1;
                        for (let row of rows){
                          // console.log(row.file_Name);
                          array[i] = "Event#: "+j+"  Summary: "+row.summary+" Start date/time: "+row.start_time;
                          // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time);
                          console.log(array[i]);
                          i++;
                          j++;
                        }
                        let jsonstr = JSON.stringify(array);
                        // console.log(jsonstr);
                        res.send(jsonstr);
//

                      }
                    });
                    // array[i] = "Event#: "+j+"  Summary: "+row.summary+" Start date/time: "+row.start_time+" Location: "+row.location+" Organizer: "+row.organizer+" file_Name: "+row.cal_file;
                    // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
                    // console.log(array[i]);
                    i++;
                    j++;
                }
              }
                // if(i==0)
                // {
                //
                //   let jsonstr = JSON.stringify(array);
                //   console.log(jsonstr);
                //   res.send(jsonstr);
                //
                // }


          }
      });


    });

    app.get('/q3',function(req , res){

        let array = [];
        let i = 0;
        let j =0;


        connection.query("SELECT DISTINCT start_time FROM EVENT ORDER BY start_time", function (err, rows, fields) {
          if (err){
              console.log("Something went wrong. "+err);
              let jsonstr = JSON.stringify(array);
    //           // console.log(jsonstr);
              res.send(jsonstr);
            }
          else {
      //         // console.log("EVENT contents sorted by startdat:");
      //             i= 0;
      //             j=1;
                  for (let row of rows){
                    // function formatDate(date) {
                        // var d = new Date(date),
                        let month = '' + (row.start_time.getMonth() + 1);
                        let day = '' + row.start_time.getDate();
                        let year = row.start_time.getFullYear();

                        if (month.length < 2) month = '0' + month;
                        if (day.length < 2) day = '0' + day;

                        let fullDate = [year, month, day].join('/');

                        let hour = row.start_time.getHours();
                        let min = row.start_time.getMinutes();
                        let sec = row.start_time.getSeconds();

                        if(hour < 10) {
                            hour = '0'+hour;
                        }
                        if(min < 10) {
                            min= '0'+ min;
                        }
                        if(sec < 10) {
                            sec= '0'+ sec;
                        }

                        fullDate = fullDate+" "+hour+":"+min+":"+sec;
                    // }
                    // let datetimeStr = row.start_time.toString().slice(0,10)+" "+row.start_time.toString().slice(11,19);
                    // datetimeStr = datetimeStr.slice(0,10)+" "+datetimeStr.slice(11,19);
                     // obj[j].startDT.date.slice(0,4)+"-"+obj[j].startDT.date.slice(4,6)
                     console.log(row.start_time);
                    console.log(fullDate);

                    // WHERE DateTime >= '12/04/2011 12:00:00 AM' AND DateTime <= '25/05/2011 3:53:04 AM'
                    // YYYY-MM-DD HH:MI:SS
                    connection.query("SELECT * FROM EVENT WHERE start_time = \'"+fullDate+"\'", function (err, rows, fields) {
                      // connection.query("SELECT * FROM EVENT WHERE start_time >= \'1900/01/01 12:00:00\' AND start_time <= \'2000/01/01 3:53:04\'", function (err, rows, fields) {

                        if (err)
                            console.log("Something went wrong. "+err);
                        else {
                            // console.log("EVENT contents sorted by startdat:");
                            console.log("outside if" + rows.length);
                            if(rows.length > 1){
                                console.log("inside if");

                                // i= 0;
                                array[0] = "Events with start time: " + row.start_time;
                                j=1;
                                for (let rowE of rows){
                                  array[j] = "Event#: "+j+"  Summary: "+rowE.summary+" Organizer: "+rowE.organizer;
                                  // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
                                  // console.log(array[j]);
                                  // i++;
                                  j++;
                              }
                              let jsonstr = JSON.stringify(array);
                              // console.log(jsonstr);
                              res.send(jsonstr);
                            }


                        }
                    });

      //               array[i] = row.start_time+"----Event#: "+j+"  Summary: "+row.summary+" Organizer: "+row.organizer;
      //               console.log(array[i]);
      //               i++;
      //               j++;
                }
      //           let jsonstr = JSON.stringify(array);
      // //           // console.log(jsonstr);
      //           res.send(jsonstr);
      //

          }
      });



    });

    app.get('/q4',function(req , res){

        let array = [];
        let i = 0;
        let j =0;

        let currDate = new Date();

        let month = '' + (currDate.getMonth() + 1);
        let day = '' + currDate.getDate();
        let year = currDate.getFullYear();

        if (month.length < 2) month = '0' + month;
        if (day.length < 2) day = '0' + day;

        let fullDate = [year, month, day].join('/');

        let hour = currDate.getHours();
        let min = currDate.getMinutes();
        let sec = currDate.getSeconds();

        if(hour < 10) {
            hour = '0'+hour;
        }
        if(min < 10) {
            min= '0'+ min;
        }
        if(sec < 10) {
            sec= '0'+ sec;
        }

        fullDate = fullDate+" "+hour+":"+min+":"+sec;

        console.log(fullDate);




         // connection.query("SELECT COUNT(*) count FROM EVENT WHERE start_time > \'"+fullDate+"\'", function (err, rows, fields) {
        // connection.query("SELECT COUNT(*) count FROM EVENT WHERE start_time > \'"+fullDate+"\'", function (err, rows, fields) {
          connection.query("SELECT *, COUNT(*) count  FROM EVENT WHERE start_time >= \'"+fullDate+"\'", function (err, rows, fields) {


          if (err)
          {
              console.log("Something went wrong. "+err);
              let jsonStrReturn = "{\"error\":\"0\"}";
              res.send(jsonStrReturn);
            }
          else {
              console.log("Count");
              let total=0;

      //             i= 0;
      //             j=1;
                  for (let row of rows){
                    total = row.count;
      //               array[i] = "Event#: "+j+"  Summary: "+row.summary+" Start date/time: "+row.start_time+" Location: "+row.location+" Organizer: "+row.organizer+" file_Name: "+row.cal_file;
      //               // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
                      console.log(row.summary+ total);
                    // console.log(row.count);
      //               i++;
      //               j++;
                }
                let jsonStrReturn = "{\"error\":\""+total+"\"}";
                res.send(jsonStrReturn);
                // let jsonstr = JSON.stringify(array);
      //           // console.log(jsonstr);
      //           res.send(jsonstr);
      //
      //
          }
      });
    //   let jsonstr = JSON.stringify(array);
    // // //           // console.log(jsonstr);
    //           res.send(jsonstr);


    });


    app.get('/q5',function(req , res){

        let array = [];
        let i = 0;
        let j =0;

        let currDate = new Date();

        let month = '' + (currDate.getMonth() + 1);
        let day = '' + currDate.getDate();
        let year = currDate.getFullYear();

        if (month.length < 2) month = '0' + month;
        if (day.length < 2) day = '0' + day;

        let fullDate = [year, month, day].join('/');

        let hour = currDate.getHours();
        let min = currDate.getMinutes();
        let sec = currDate.getSeconds();

        if(hour < 10) {
            hour = '0'+hour;
        }
        if(min < 10) {
            min= '0'+ min;
        }
        if(sec < 10) {
            sec= '0'+ sec;
        }

        fullDate = fullDate+" "+hour+":"+min+":"+sec;

        console.log(fullDate);




         // connection.query("SELECT COUNT(*) count FROM EVENT WHERE start_time > \'"+fullDate+"\'", function (err, rows, fields) {
        // connection.query("SELECT COUNT(*) count FROM EVENT WHERE start_time > \'"+fullDate+"\'", function (err, rows, fields) {
          connection.query("SELECT * FROM EVENT WHERE start_time < \'"+fullDate+"\'", function (err, rows, fields) {


          if (err)
          {
              console.log("Something went wrong. "+err);
              let jsonstr = JSON.stringify(array);
    //           // console.log(jsonstr);
              res.send(jsonstr);
            }
          else {
              console.log("Count");
              let total=0;

                  i= 0;
                  j=1;
                  for (let row of rows){

                    array[i] = "Event#: "+j+"  Summary: "+row.summary+", Start date/time: "+row.start_time+", Location: "+row.location+", Organizer: "+row.organizer+", file_Name: "+row.cal_file;
      //               // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
                      // console.log(row.summary+ total);
                    // console.log(row.count);
                    i++;
                    j++;
                }
                // let jsonStrReturn = "{\"error\":\""+total+"\"}";
                // res.send(jsonStrReturn);
                let jsonstr = JSON.stringify(array);
      //           // console.log(jsonstr);
                res.send(jsonstr);
      //
      //
          }
      });
    //   let jsonstr = JSON.stringify(array);
    // // //           // console.log(jsonstr);
    //           res.send(jsonstr);


    });

    app.get('/q6',function(req , res){

      // let filename = req.query.filename;

        let array = [];
        let i = 0;
        let j =1;

        connection.query("SELECT * FROM ALARM WHERE action = \'AUDIO\'", function (err, rows, fields) {


        if (err)
        {
            console.log("Something went wrong. "+err);
            let jsonstr = JSON.stringify(array);
  //           // console.log(jsonstr);
            res.send(jsonstr);
          }
        else {
                i= 0;
                j=1;
                for (let row of rows){

                  array[i] = "Alarms#: "+j+"  Action: "+row.action+", Trigger: "+row.trigger;
    //               // console.log("Event#: "+i+"  summary: "+row.summary+" start_time: "+row.start_time+" location: "+row.location+" organizer: "+row.organizer+" cal_file: "+row.cal_file );
                    // console.log(row.summary+ total);
                  // console.log(row.count);
                  i++;
                  j++;
              }
              // let jsonStrReturn = "{\"error\":\""+total+"\"}";
              // res.send(jsonStrReturn);
              let jsonstr = JSON.stringify(array);
    //           // console.log(jsonstr);
              res.send(jsonstr);
    //
    //
        }
    });



    });







app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
