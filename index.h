const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html>

<head>
	<title>RealTime Graph|RoboSoc</title>

	<script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>	
	<style>
	canvas{
		-moz-user-select: none;
		-webkit-user-select: none;
		-ms-user-select: none;
	}

	/* Data Table Styling */
	#dataTable {
	  font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
	  border-collapse: collapse;
	  width: 100%;
	}

	#dataTable td, #dataTable th {
	  border: 1px solid #ddd;
	  padding: 8px;
	}

	#dataTable tr:nth-child(even){background-color: #f2f2f2;}

	#dataTable tr:hover {background-color: #ddd;}

	#dataTable th {
	  padding-top: 12px;
	  padding-bottom: 12px;
	  text-align: left;
	  background-color: #4CAF50;
	  color: white;
	}
	</style>
</head>

<body>
    <div style="text-align:center;"><br>Real Time Data Logging with Graphs on NodeMCU</div>
    <div class="chart-container" position: relative; height:350px; width:100%">
        <canvas id="Chart" width="400" height="400"></canvas>
    </div> <div>
	<input id="inputFileNameToSaveAs"></input>
    <button onclick="saveTextAsFile()">Save Text to File</button>
	<table id="dataTable">
	  <tr><th>Date-Time</th><th>Left RPM</th><th>Right RPM</th><th>UltraSonic Distance</th></tr>
	</table> </div> <br> <br> 

<script type="text/javascript">
var valuesUS = [];
var timeStamp = [];
var entry="";
function showGraph() {
    for (i = 0; i < arguments.length; i++) {
    	valuesUS.push(arguments[i]);
    }

    var ctx = document.getElementById("Chart").getContext('2d');
    var Chart2 = new Chart(ctx, {
        type: 'line',
        data: {
            labels: timeStamp,	//Bottom Labeling
            datasets: [{
                label: "Distance",
                fill: false,	//Try with true
                backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
                borderColor: 'rgba( 243, 156, 18 , 1)',	//Graph Line Color
                data: valuesUS,
            }],
        },
        options: {
            title: {
                    display: true,
                    text: "UltraSonic"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });

}

//On Page load show graphs 
window.onload = function() {
	console.log(new Date().toLocaleTimeString());
	showGraph(0,0,0,0);
};


setInterval(function() {
  // Call a function repetatively with 5Second interval
  getData();
}, 500); //500mSeconds update rate

function saveTextAsFile() {
    var textToSave = entry;
    var textToSaveAsBlob = new Blob([textToSave], {type:"text/plain"});
    var textToSaveAsURL = window.URL.createObjectURL(textToSaveAsBlob);
    var fileNameToSaveAs = document.getElementById("inputFileNameToSaveAs").value;

    var downloadLink = document.createElement("a");
    downloadLink.download = fileNameToSaveAs;
    downloadLink.innerHTML = "Download File";
    downloadLink.href = textToSaveAsURL;
    downloadLink.onclick = destroyClickedElement;
    downloadLink.style.display = "none";
    document.body.appendChild(downloadLink);

    downloadLink.click();
}

function destroyClickedElement(event) {
    document.body.removeChild(event.target);
}

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
	var date = new Date().toLocaleDateString();
	var time = new Date().toLocaleTimeString();
	var filedata = String(this.responseText);
	var s1 = filedata.indexOf('#');
	var s2 = filedata.indexOf('%');
	var l = filedata.length;	
	var IR1 = filedata.substring(0,s1);
	var IR2 = filedata.substring(s1+1,s2);
	var US = filedata.substring(s2+1,l);      

      valuesUS.push(US);		
      timeStamp.push(date+" "+time);
      showGraph();	//Update Graphs
	//Update Data Table
	  var table = document.getElementById("dataTable");
	  var row = table.insertRow(1);	//Add after headings
	  entry = entry+"TIME:"+time+" "+"IR1:"+IR1+" "+"IR2:"+IR2+" "+"US:"+US+"\r\n";
	  var cell1 = row.insertCell(0);
	  var cell2 = row.insertCell(1);
	  var cell3 = row.insertCell(2);
	  var cell4 = row.insertCell(3);
	  cell1.innerHTML = date+"   "+time;
	  cell2.innerHTML = IR1;
	  cell3.innerHTML = IR2;
	  cell4.innerHTML = US;	
    }
  };
  xhttp.open("GET", "readADC", true);	//Handle readADC server on ESP8266
  xhttp.send();
}

</script> 

</body>

</html>

)=====";
