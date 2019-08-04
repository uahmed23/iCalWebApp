// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

  let allFilenames = [];
  var scrollelem;
  var valFile = [];
  //



  $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/filenames',   //The server endpoint we are connecting to
      success: function (data) {

          for (i =0; i < data.length; i++)
          {

            console.log(data[i]);
            allFilenames[i] = data[i];

          }
      },
      fail: function(error) {
          // Non-200 return, do something with error
          console.log(error);
      }
  });



    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string,
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, Received string '"+JSON.stringify(data)+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the
        $.ajax({});
    });

    $('#clearButton').click(function(e){
      $('#statusTable tbody > tr').remove();

    });




   //  $("#calDropdown").change(function(){
   //     var selectedCountry = $(this).children("option:selected").val();
   //     alert("You have selected the country - " + selectedCountry);
   // });
// function fileUpdate(){
    $.ajax({

        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything
        url: '/object',   //The server endpoint we are connecting to
        // data: {filename:filename},
        success: function (data) {
          var obj;
          var i,j,k, Tfiles =0;
          var tableRef;
          var file = [];
          var isValid;

          if(data.length == 0)
          {
            $("#filelogTable").append("<tr><th scope=\"row\"> <a>No files</a></th></tr> ");

          }
          for (i =0; i < data.length; i++)
          {
            obj = JSON.parse(data[i]);
            if(data[i] != "{}")
            {
              $("#filelogTable").append("<tr><th scope=\"row\"> <a href=\"/uploads/"+obj.filename+"\">"+obj.filename+"</a></th><td>"+obj.version+"</td> <td>"+obj.prodID+"</td><td>"+obj.numEvents+"</td><td>"+obj.numProps+"</td>  </tr> ");
              // $("#calDropdown").append("<a id=\""+obj.filename +"\" class=\"dropdown-item\" href=\"#\">"+obj.filename+"</a>");
              $("#statusMessages").append("<tr><th scope=\"row\">Successfully uploaded "+obj.filename+"</th></tr>");
              scrollelem = document.getElementById("stdiv");
              scrollelem.scrollTop = scrollelem.scrollHeight;
              $("#calDropdown").append("<option value="+obj.filename+">"+obj.filename+"</option>");
              $("#eventDropdown").append("<option value="+obj.filename+">"+obj.filename+"</option>");

              file[Tfiles] = obj.filename;
              Tfiles++;

            }
          }
          valFile = file;

            for(j=0; j < allFilenames.length; j++)
            {
              isValid = false;
              for(k=0;k<file.length; k++)
              {
                if(allFilenames[j]==file[k])
                {
                  isValid = true
                }

              }
              if(isValid == false)
              {
                $("#statusMessages").append("<tr><th scope=\"row\">Invalid File:"+allFilenames[j]+"</th></tr>");
                scrollelem = document.getElementById("stdiv");
                scrollelem.scrollTop = scrollelem.scrollHeight;
              }

            }
            // $("#statusMessages").append("<tr><th scope=\"row\">Invaild file in uploads folder</th></tr>");





        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error);
        }
    });

// }
// fileUpdate();

    var dropdownFile;

    $('#calDropdown').click(function(e){

      dropdownFile = $(this).children("option:selected").val();


      $.ajax({
          type: 'get',            //Request type
          dataType: 'json',       //Data type - we will use JSON for almost everything
          url: '/events',   //The server endpoint we are connecting to
          success: function (data) {
            var obj;
            var i,j,num;
            var numProps;


            for (i =0; i < data.length; i++)
            {
              obj = JSON.parse(data[i]);
              if(data[i] != "[]")
              {
                if(obj[0].filename == dropdownFile)
                {
                  $('#cvp tbody > tr').remove();
                  for (j in obj ) {

                    // num = (+j)+1;
                    // console.log(obj[j].summary);
                    // $("#cvp").append("<tr><th scope=\"row\">"+j+"</th><td>"obj[j].date"</td><td>"+obj[j].time+"</td><td>"+obj[j].summary+"</td><td>"+obj[j].numProps+"</td><td>"+obj[j].numAlarms+"</td></tr> ");

                    if(j!=0)
                    {
                      numProps = (+obj[j].numProps);
                      // console.log(numProps);
                      // $("#cvp").append("<tr><td class=\"ne\">"+j+"</td><td>"+obj[j].startDT.date+"</td><td>"+obj[j].startDT.time+"</td><td>"+obj[j].summary+"</td><td><button id=\"showPropBttn\" type=\"button\" class=\"use-address\">"+obj[j].numProps+"</button></td> </td><td><button id=\"showAlrmBttn\" type=\"button\" class=\"btn btn-outline-info\">"+obj[j].numAlarms+"</button></td> ");
                      // if(obj[j].starD)
                      if(obj[j].startDT.isUTC == true)
                      {
                        $("#cvp").append("<tr><th scope=\"row\"  class=\"ne\">"+j+"</th><td>"+obj[j].startDT.date.slice(0,4)+"/"+obj[j].startDT.date.slice(4,6)+"/"+obj[j].startDT.date.slice(6,8)+"</td><td>"+obj[j].startDT.time.slice(0,2)+":"+obj[j].startDT.time.slice(2,4)+":"+obj[j].startDT.time.slice(4,6)+" (UTC)</td><td>"+obj[j].summary+"</td><td><button id=\"showPropBttn\" type=\"button\" class=\"btn btn-outline-info\">"+numProps +"</button></td> </td><td><button id=\"showAlrmBttn\" type=\"button\" class=\"btn btn-outline-info\">"+obj[j].numAlarms+"</button></td> ");
                      }
                      else
                      {
                          $("#cvp").append("<tr><th scope=\"row\"  class=\"ne\">"+j+"</th><td>"+obj[j].startDT.date.slice(0,4)+"/"+obj[j].startDT.date.slice(4,6)+"/"+obj[j].startDT.date.slice(6,8)+"</td><td>"+obj[j].startDT.time.slice(0,2)+":"+obj[j].startDT.time.slice(2,4)+":"+obj[j].startDT.time.slice(4,6)+" </td><td>"+obj[j].summary+"</td><td><button id=\"showPropBttn\" type=\"button\" class=\"btn btn-outline-info\">"+umProps +"</button></td> </td><td><button id=\"showAlrmBttn\" type=\"button\" class=\"btn btn-outline-info\">"+obj[j].numAlarms+"</button></td> ");

                      }
                    }

                  }
                }

              }

            }

          },
          fail: function(error) {
              // Non-200 return, do something with error
              console.log(error);
          }
      });
    });



    //SHOW ALARMS--------------------------------------------------------------------------------------------------------------------------------
    $(document).on("click","#cvp tbody tr td #showAlrmBttn", function() { // any button

        var $row = $(this).closest("tr");    // Find the row
        var numEvent = $row.find(".ne").text(); // Find the text
        // $("#statusMessages").append("<tr><th scope=\"row\">"+numEvent+"</th></tr>");


        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything
            url: '/alarms',   //The server endpoint we are connecting to
            data: {
              filename:dropdownFile,
              numEvent:numEvent
            },

            success: function (data) {
              // console.log(data[1].action);
              var obj;
              var j,num;

              if(data != "[]")
              {
                $("#statusMessages").append("<tr><th scope=\"row\">Displaying Alarms for "+dropdownFile+"\'s Event "+numEvent+":</th></tr>");
                scrollelem = document.getElementById("stdiv");
                scrollelem.scrollTop = scrollelem.scrollHeight;
                // obj = JSON.parse(data);
                for (j in data )
                {
                  // var elem = document.getElementById('#statusMessages');
                  // elem.scrollTop = elem.scrollHeight;
                  num = (+j)+1;
                  $("#statusMessages").append("<tr><th scope=\"row\">Alarm "+num+": Action:"+data[j].action+", Trigger:"+data[j].trigger+", Number of Properties:"+data[j].numProps+"</th></tr>");
                  scrollelem = document.getElementById("stdiv");
                  scrollelem.scrollTop = scrollelem.scrollHeight;
                }
              }


            },
            fail: function(error) {
                // Non-200 return, do something with error
                console.log(error);
            }
        });
      });

      //SHOW PROPERTIES----------------------------------------------------------------------------------------------------------------------------
      $(document).on("click","#cvp tbody tr td #showPropBttn", function() { // any button

          var $Prow = $(this).closest("tr");    // Find the row
          var PnumEvent = $Prow.find(".ne").text(); // Find the text
          // $("#statusMessages").append("<tr><th scope=\"row\">"+PnumEvent+"</th></tr>");

          $.ajax({
              type: 'get',            //Request type
              dataType: 'json',       //Data type - we will use JSON for almost everything
              url: '/properties',   //The server endpoint we are connecting to
              data: {
                filename:dropdownFile,
                numEvent:PnumEvent
              },

              success: function (data) {
                // console.log(data[1].action);
                var obj;
                var j,num;

                if(data != "[]")
                {
                  $("#statusMessages").append("<tr><th scope=\"row\">Displaying Properties for "+dropdownFile+"\'s Event "+PnumEvent+":</th></tr>");
                  scrollelem = document.getElementById("stdiv");
                  scrollelem.scrollTop = scrollelem.scrollHeight;
                  // obj = JSON.parse(data);
                  for (j in data )
                  {
                    num = (+j)+1;
                    $("#statusMessages").append("<tr><th scope=\"row\">Property "+num+": Property Name:"+data[j].propName+", Property Description:"+data[j].propDescr+"</th></tr>");
                    scrollelem = document.getElementById("stdiv");
                    scrollelem.scrollTop = scrollelem.scrollHeight;
                  }
                }


              },
              fail: function(error) {
                  // Non-200 return, do something with error
                  console.log(error);
              }
          });

          // alert($text);
          // console.log($(this).val());
        });

        //CREATE CALENDAR--------------------------------------------------------------------------------------------------------------------------

        // $('#CreateCalButton').click(function(e){
      $(document).on("click","#CreateCalButton", function() {

          // console.log($('#validationDefault05').text);
          var filenameExist = false;
          var extension;
          var filename = document.getElementById("CCfilename").value;
          var version = document.getElementById("CCversion").value;
          var prodid = document.getElementById("CCprodid").value;
          var CalJSONobj = {
            "version"  :  (+version),
            "prodID"   :  prodid
          }
          var calJSONstr = JSON.stringify(CalJSONobj);
          var eventuid = document.getElementById("CCeventuid").value;
          var eventsummary = document.getElementById("CCeventsummary").value;
          var EventJSONobj = {
            "UID"  :  eventuid
          }
          var eventJSONstr = JSON.stringify(EventJSONobj);

          var eventSDtemp = document.getElementById("eventSD").value;
          var eventSD = eventSDtemp.slice(0,4)+eventSDtemp.slice(5,7)+eventSDtemp.slice(8,10)

          var eventSTtemp = document.getElementById("eventST").value;
          var eventST = eventSTtemp.slice(0,2)+eventSTtemp.slice(3,5)+"00";

          var eventUTC = document.getElementById("eventUTC").checked;



          let currDate = new Date();
           var day = currDate.getDate();
           var month = currDate.getMonth()+1; //Months a 0-indexed
           var year = currDate.getFullYear();
           if(day < 10) {
               day = '0'+day;
           }
           if(month < 10) {
               month = '0'+month;
           }

           let eventCD = year + month + day;


           var hour = currDate.getHours();
           var min = currDate.getMinutes();
           var sec = currDate.getSeconds();

           if(hour < 10) {
               hour = '0'+hour;
           }
           if(min < 10) {
               min= '0'+ min;
           }
           if(sec < 10) {
               sec= '0'+ sec;
           }

           // console.log(hour+":"+min+":"+sec);

           let eventCT = hour+""+min+""+sec;

           // console.log(eventCD+"/"+eventCT);
           filenameExist = false;
           for(j=0; j < allFilenames.length; j++)
           {

               if(allFilenames[j]==filename)
               {
                 filenameExist = true;
               }
           }

           if(filenameExist == true)
           {
             $("#statusMessages").append("<tr><th scope=\"row\">File Already Exists in ./uploads: "+ filename+"</th></tr>");
             scrollelem = document.getElementById("stdiv");
             scrollelem.scrollTop = scrollelem.scrollHeight;
           }

           var filenamelength = filename.length;
           extension = filename.slice(filenamelength-4,filenamelength);
           // console.log(extension);
           if(extension != ".ics")
           {
             filenameExist = true;
             $("#statusMessages").append("<tr><th scope=\"row\">("+ filename+ ")Incorrect .ics file extension: file not created. </th></tr>");
             scrollelem = document.getElementById("stdiv");
             scrollelem.scrollTop = scrollelem.scrollHeight;
             // console.log(filename+" "+ extension);
           }

// num = (+j)+1;

          // console.log(filename);
          if(filename.length != 0 && version.length!=0 && prodid.length!=0 && eventuid.length!=0 && eventSD.length!=0 && eventST.length!=0 && filenameExist == false)
          {

            $.ajax({
                type: 'get',            //Request type
                dataType: 'json',       //Data type - we will use JSON for almost everything
                url: '/createCal',   //The server endpoint we are connecting to
                data: {
                  filename:filename,
                  calJSON:calJSONstr,
                  eventJSON:eventJSONstr,
                  eventsummary:eventsummary,
                  eventSD:eventSD,
                  eventST:eventST,
                  eventUTC:eventUTC,
                  eventCD:eventCD,
                  eventCT:eventCT,
                },

                success: function (data) {

                  // $('#statusTable tbody > tr').remove();
                  // fileUpdate();

                  $("#statusMessages").append("<tr><th scope=\"row\">Creating Calendar...</th></tr>");
                  $("#statusMessages").append("<tr><th scope=\"row\">("+ filename +") "+data.error+"</th></tr>");
                  scrollelem = document.getElementById("stdiv");
                  scrollelem.scrollTop = scrollelem.scrollHeight;
                  // document.location.reload();
                  // console.log(data.error);
                  // console.log("heloo");
                },
                fail: function(error) {
                    // Non-200 return, do something with error

                    console.log(error);
                }
            });
          }
        });


        //ADD EVENT_______________________________________________________________________________________________________////__//_/__./''./
        var CEfilename = "";

        $('#eventDropdown').click(function(e){

          CEfilename = $(this).children("option:selected").val();
        });


          $(document).on("click","#CreateEventButton", function() {

              // console.log($('#validationDefault05').text);
              // var choseFile= false;
              // var CEfilename = document.getElementById("CCfilename").value;//

              var CEeventuid = document.getElementById("CEeventuid").value;
              var CEeventsummary = document.getElementById("CEeventsummary").value;
              var CEEventJSONobj = {
                "UID"  :  CEeventuid
              }
              var CEeventJSONstr = JSON.stringify(CEEventJSONobj);

              var CEeventSDtemp = document.getElementById("CEeventSD").value;
              var CEeventSD = CEeventSDtemp.slice(0,4)+CEeventSDtemp.slice(5,7)+CEeventSDtemp.slice(8,10)

              var CEeventSTtemp = document.getElementById("CEeventST").value;
              var CEeventST = CEeventSTtemp.slice(0,2)+CEeventSTtemp.slice(3,5)+"00";

              var CEeventUTC = document.getElementById("CEeventUTC").checked;



              let currDate = new Date();
               var day = currDate.getDate();
               var month = currDate.getMonth()+1; //Months a 0-indexed
               var year = currDate.getFullYear();
               if(day < 10) {
                   day = '0'+day;
               }
               if(month < 10) {
                   month = '0'+month;
               }

               let CEeventCD = year + month + day;


               var hour = currDate.getHours();
               var min = currDate.getMinutes();
               var sec = currDate.getSeconds();

               if(hour < 10) {
                   hour = '0'+hour;
               }
               if(min < 10) {
                   min= '0'+ min;
               }
               if(sec < 10) {
                   sec= '0'+ sec;
               }

               // console.log(hour+":"+min+":"+sec);

               let CEeventCT = hour+""+min+""+sec;


              if(CEfilename.length != 0 && CEfilename != "Filename (required)" && CEeventuid.length!=0 && CEeventSD.length!=0 && CEeventST.length!=0)
              {

                $.ajax({
                    type: 'get',            //Request type
                    dataType: 'json',       //Data type - we will use JSON for almost everything
                    url: '/addEvent',   //The server endpoint we are connecting to
                    data: {
                      filename:CEfilename,
                      eventJSON:CEeventJSONstr,
                      eventsummary:CEeventsummary,
                      eventSD:CEeventSD,
                      eventST:CEeventST,
                      eventUTC:CEeventUTC,
                      eventCD:CEeventCD,
                      eventCT:CEeventCT,
                    },


                    success: function (data) {


                      $("#statusMessages").append("<tr><th scope=\"row\">Adding event to "+CEfilename+"</th></tr>");
                      $("#statusMessages").append("<tr><th scope=\"row\">("+ CEfilename +") "+data.error+"</th></tr>");

                        scrollelem = document.getElementById("stdiv");
                        scrollelem.scrollTop = scrollelem.scrollHeight;
                    },
                    fail: function(error) {
                        // Non-200 return, do something with error

                        console.log(error);
                    }
                });
              }
            });



       // A4 FUNCTIONALITY__________________________________________________________________________________________________________________________
       let username;
       let password;
       let dbname;

       $(document).on("click","#dbButton", function() {


         username = document.getElementById("dbusername").value;
         password = document.getElementById("dbpassword").value;
         dbname = document.getElementById("dbname").value;

         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/dbconnection',   //The server endpoint we are connecting to
             data: {
               username:username,
               password:password,
               dbname:dbname,
             },

             success: function (data) {

               console.log(data.error+" because function returns undefined!");

               if(data.error == "fail")
               {
                  var modal = document.getElementById('loginModal');
                  // var btn = document.getElementById("dbButton");
                  var span = document.getElementsByClassName("close")[0];
                  // btn.onclick = function() {
                    modal.style.display = "block";
                  // }
                  span.onclick = function() {
                    modal.style.display = "none";
                  }
                  window.onclick = function(event) {
                    if (event.target == modal) {
                      modal.style.display = "none";
                    }
                  }


                  $('#displaydbButton').attr("disabled", true);
                  $('#cleardbButton').attr("disabled", true);
                  $('#queryButton1').attr("disabled", true);
                  $('#queryButton2').attr("disabled", true);
                  $('#queryButton3').attr("disabled", true);
                  $('#queryButton4').attr("disabled", true);
                  $('#queryButton5').attr("disabled", true);
                  $('#queryButton6').attr("disabled", true);
                  $('#storedbButton').attr("disabled", true);
               }
               else
               {
                   // $('#dbButton').attr("disabled", true);

                   $('#displaydbButton').attr("disabled", false);
                   $('#cleardbButton').attr("disabled", false);
                   $('#queryButton1').attr("disabled", false);
                   $('#queryButton2').attr("disabled", false);
                   $('#queryButton3').attr("disabled", false);
                   $('#queryButton4').attr("disabled", false);
                   $('#queryButton5').attr("disabled", false);
                   $('#queryButton6').attr("disabled", false);
                   $('#storedbButton').attr("disabled", false);
               }

               console.log(username+" "+password+" "+dbname);

               // $("#statusMessages").append("<tr><th scope=\"row\">Adding event to "+CEfilename+"</th></tr>");
               // $("#statusMessages").append("<tr><th scope=\"row\">("+ CEfilename +") "+data.error+"</th></tr>");
               //
               //   scrollelem = document.getElementById("stdiv");
               //   scrollelem.scrollTop = scrollelem.scrollHeight;
             },
             fail: function(error) {

                 console.log(error);
             }
         });

       });

       //CLEAR ALL DATA
       $(document).on("click","#cleardbButton", function() {

         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/cleardb',   //The server endpoint we are connecting to


             success: function (data) {
               details();


             },
             fail: function(error) {

                 console.log(error);
             }
         });

       });

       //STORE ALL FILES
       $(document).on("click","#storedbButton", function() {
         // var counter;

         // for(counter=0; counter < valFile.length; counter++)
         // {
           // console.log(valFile)
           $.ajax({
               type: 'get',            //Request type
               dataType: 'json',       //Data type - we will use JSON for almost everything
               url: '/storedb',   //The server endpoint we are connecting to
               data: {
                 filenames:valFile,

               },


               success: function (data) {

                     console.log("store file: "+data.error);

                     $.ajax({
                         type: 'get',            //Request type
                         dataType: 'json',       //Data type - we will use JSON for almost everything
                         url: '/storeEvents',   //The server endpoint we are connecting to
                         // data: {
                         //   filenames:valFile,
                         //
                         // },


                         success: function (data) {

                               console.log("store events: "+data.error);
                               $.ajax({
                                   type: 'get',            //Request type
                                   dataType: 'json',       //Data type - we will use JSON for almost everything
                                   url: '/storeAlarms',   //The server endpoint we are connecting to
                                   // data: {
                                   //   filenames:valFile,
                                   //
                                   // },


                                   success: function (data) {

                                         console.log("store alarms: "+data.error);
                                         details();




                                   },
                                   fail: function(error) {

                                       console.log(error);
                                   }
                               });


                         },
                         fail: function(error) {

                             console.log(error);
                         }
                     });


               },
               fail: function(error) {

                   console.log(error);
               }
           });

       });//end of onclick

       //CLEAR DISPLAY DB STATUS

       function details() {
         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/displaydb',   //The server endpoint we are connecting to


             success: function (data) {
               $("#statusMessages").append("<tr><th scope=\"row\">Database has "+data.files+" files, "+data.events+" events, and "+data.alarms+" alarms </th></tr>");

                 scrollelem = document.getElementById("stdiv");
                 scrollelem.scrollTop = scrollelem.scrollHeight;

                console.log(""+data.error+"Database has "+data.files+" files, "+data.events+" events, and "+data.alarms+" alarms ");

             },
             fail: function(error) {

                 console.log(error);
             }
         });
        }

       $(document).on("click","#displaydbButton", function() {
         // console.log("hello");
         details();

       });

       $(document).on("click","#queryButton1", function() {
         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/q1',   //The server endpoint we are connecting to
             success: function (data) {

               let i;

               $('#queryTable tbody > tr').remove();

               for (i =0; i < data.length; i++)
               {
                   $("#queryMessages").append("<tr><th scope=\"row\">"+data[i]+"</th></tr>");
               }
             },
             fail: function(error) {

                 console.log(error);
             }
         });
       });

       $(document).on("click","#queryButton2", function() {
         var q2filename = document.getElementById("q2filename").value;


         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/q2',   //The server endpoint we are connecting to
             data: {
               filename:q2filename,
             },

             success: function (data) {

               let i;

               if(data.length == 0)
               {
                 $('#queryTable tbody > tr').remove();
                 $("#queryMessages").append("<tr><th scope=\"row\">Invalid Filename</th></tr>");

               }
               else {
                 $('#queryTable tbody > tr').remove();

                 for (i =0; i < data.length; i++)
                 {
                   $("#queryMessages").append("<tr><th scope=\"row\">"+data[i]+"</th></tr>");
                 }

               }

             },
             fail: function(error) {

                 console.log(error);
             }
         });
       });


       $(document).on("click","#queryButton3", function() {
         console.log("q3");

         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/q3',   //The server endpoint we are connecting to


             success: function (data) {

               let i;

               if(data.length == 0)
               {
                 $('#queryTable tbody > tr').remove();
                 $("#queryMessages").append("<tr><th scope=\"row\">No conflicting events.</th></tr>");

               }
               else {
                 $('#queryTable tbody > tr').remove();
                 $("#queryMessages").append("<tr><th scope=\"row\">Conflicting events.</th></tr>");

                 for (i =0; i < data.length; i++)
                 {
                   $("#queryMessages").append("<tr><th scope=\"row\">"+data[i]+"</th></tr>");
                 }

               }

             },
             fail: function(error) {

                 console.log(error);
             }
         });
       });

       $(document).on("click","#queryButton4", function() {
         console.log("q4");

         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/q4',   //The server endpoint we are connecting to


             success: function (data) {

               console.log("store file: "+data.error);


               // let i;
               //
               if(data.error == 0)
               {
                 $('#queryTable tbody > tr').remove();
                 $("#queryMessages").append("<tr><th scope=\"row\">0 upcoming events</th></tr>");

               }
               else {
                 $('#queryTable tbody > tr').remove();
                 $("#queryMessages").append("<tr><th scope=\"row\">"+data.error+" upcoming events</th></tr>");


               }

             },
             fail: function(error) {

                 console.log(error);
             }
         });
       });

       $(document).on("click","#queryButton5", function() {
         console.log("q5");

         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/q5',   //The server endpoint we are connecting to


             success: function (data) {


                let i;

                if(data.length == 0)
                {
                  $('#queryTable tbody > tr').remove();
                  $("#queryMessages").append("<tr><th scope=\"row\">No past events.</th></tr>");

                }
                else {
                  $('#queryTable tbody > tr').remove();
                  $("#queryMessages").append("<tr><th scope=\"row\">Past Events:</th></tr>");

                  for (i =0; i < data.length; i++)
                  {
                    $("#queryMessages").append("<tr><th scope=\"row\">"+data[i]+"</th></tr>");
                  }

                }

             },
             fail: function(error) {

                 console.log(error);
             }
         });
       });


       $(document).on("click","#queryButton6", function() {
         // var q6filename = document.getElementById("q6filename").value;


         $.ajax({
             type: 'get',            //Request type
             dataType: 'json',       //Data type - we will use JSON for almost everything
             url: '/q6',   //The server endpoint we are connecting to

             success: function (data) {

               let i;

               if(data.length == 0)
               {
                 $('#queryTable tbody > tr').remove();
                 $("#queryMessages").append("<tr><th scope=\"row\">No alarms with AUDIO action.</th></tr>");

               }
               else {
                 $('#queryTable tbody > tr').remove();

                 $("#queryMessages").append("<tr><th scope=\"row\">Alarms with AUDIO action.</th></tr>");
                 for (i =0; i < data.length; i++)
                 {
                   $("#queryMessages").append("<tr><th scope=\"row\">"+data[i]+"</th></tr>");
                 }

               }

             },
             fail: function(error) {

                 console.log(error);
             }
         });
       });




});
