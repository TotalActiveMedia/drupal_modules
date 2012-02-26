 <html>
 <head>
    <title>Norvegiana JSONP test</title>

    <script src="http://api.simile-widgets.org/exhibit/2.2.0/exhibit-api.js"
            type="text/javascript"></script>
<script src="http://static.simile.mit.edu/exhibit/extensions-2.0/time/time-extension.js"
     type="text/javascript"></script> 
	 <script type="text/javascript" src="http://api.simile-widgets.org/exhibit/2.2.0/extensions/map/map-extension.js?gmapkey=ABQIAAAAOzjnxSh6DGJAeUM7wGYo6hQIDzCu4gYihK_Kj-yyuboyapyMhhSyUfDIFNa3VPzWaGr8kFg4A6ARUQ"></script>

	 <script type="text/javascript">
	 function eirikmap(mapDiv){ 
	   map = new GMap2(mapDiv); 
	   map.setCenter(new GLatLng(65.439209,5.973408), 4); 
	   map.addControl(new GLargeMapControl3D()); 
	   map.addControl(new GMenuMapTypeControl()); 
	   map.addControl(new GScaleControl()); 
	   map.addMapType(G_PHYSICAL_MAP); 
	   map.setMapType(G_PHYSICAL_MAP); 

	  //you can add the mouse scroll wheel too 
	   map.enableScrollWheelZoom(); 

  return map; 

}


</script>
    <style>
    </style>
 </head> 
 <body>
    <h1>Norvegiana JSONP test</h1>
    <table width="100%">
        <tr valign="top">
		
            <td ex:role="viewPanel">
        
<table ex:role="lens" class="nobelist">
       <tr>
           <td><img ex:src-content=".thumbnail" /></td>
           <td>
               <div ex:content=".label" class="name"></div>
               <div>
                   <span ex:content=".municipality" class="discipline"></span>, 
                   <span ex:content=".created" class="year"></span>
				   <span ex:content=".geography" class="year"></span>
               </div>
               
           </td>
       </tr>
   </table>

		<div ex:role="view"
     ex:viewClass="Timeline"
     ex:start=".created"
     ex:colorKey=".dc_type">
 </div>
				<div ex:role="view"></div><div ex:role="view" viewclass="Map" latlng=".geography" center="61.439209,5.973408" zoom="4" ex:mapConstructor="eirikmap"></div>
				
            </td>
            <td width="25%">
 <div ex:role="facet" 
   ex:facetClass="TextSearch"
   ex:facetLabel="Search exhibit">
   
 </div>
 <div ex:role="facet" 
   ex:expression=".type"></div>
    <div ex:role="facet" 
   ex:expression=".municipality">
 </div>  
 
 </td>
        </tr>
    </table>

<script src="delvingsearch_simile_prepare.php?query=<?php echo $_GET['query'] ?>"></script>

	</body>
 </html>
