<% do_pagehead("upnp.titl"); %>
<!--
		UPnP
		11.2005	tofu10		init
		Intergated to DD-WRT by LawnMowerGuy1
-->
	<script type="text/javascript">
	//<![CDATA[

var data = new Array();

function parseForwards(upnpForwards) {
	// wan_port0-wan_port1>lan_ipaddr:lan_port0-lan_port1,proto,enable,desc
	data = [];
	for (var i = 0; i < upnpForwards.length; ++i) {
		if (upnpForwards[i] == '' || !upnpForwards[i].match(/^(\d+-\d+)&gt;(.*?):(\d+-\d+),(.*?),(.*?),(.*)/)) continue;
		var e = {};
		e.index = i;
		e.wanPorts = RegExp.$1;
		e.lanIP = RegExp.$2;
		e.lanPorts = RegExp.$3;
		e.proto = RegExp.$4.toUpperCase();
		e.enabled = (RegExp.$5 == 'on');
		e.desc = RegExp.$6;

		if ((e.wanPorts.match(/^(\d+)-(\d+)$/)) && (RegExp.$1 == RegExp.$2)) e.wanPorts = RegExp.$1;
			else e.wanPorts = RegExp.$1 + "&nbsp;-&nbsp;" + RegExp.$2;
		if ((e.lanPorts.match(/^(\d+)-(\d+)$/)) && (RegExp.$1 == RegExp.$2)) e.lanPorts = RegExp.$1;
			else e.lanPorts = RegExp.$1 + "&nbsp;-&nbsp;" + RegExp.$2;
		data.push(e);
	}
}

function setUPnPTable(forwards) {
	parseForwards(forwards);
	var table = document.getElementById("upnp_table");
	for(var i = table.rows.length - 1; i > 0 ; i--) {
		table.deleteRow(i);
	}
	if(data.length == 0) {
		var cell = table.insertRow(-1).insertCell(-1);
		cell.colSpan = 6;
		cell.align = "center";
		cell.innerHTML = "- " + share.none + " - ";
		document.getElementsByName("delete_button")[0].disabled = true;
		return;
	} else {
			document.getElementsByName("delete_button")[0].disabled = false;
	}
	for(var i = 0; i < data.length; i++) {
		var row = table.insertRow(-1);
		//IP
		var cell = row.insertCell(-1);
		cell.innerHTML = data[i].lanIP;
		cell.align="left";

		//wan port
		var cell = row.insertCell(-1);
		cell.innerHTML = data[i].wanPorts;
		cell.align="left";

		//lan port
		var cell = row.insertCell(-1);
		cell.innerHTML = data[i].lanPorts;
		cell.align="left";

		//proto
		var cell = row.insertCell(-1);
		cell.innerHTML = data[i].proto;
		cell.align = "left";

		row.style.height = "15px";
		row.className = (data[i].enabled ? '' : 'disabled');
		//descr
		row.insertCell(-1).innerHTML = data[i].desc;

		cell = row.insertCell(-1);
		cell.title = upnp.msg1;
		cell.align = "center";
		cell.innerHTML = "<input class=\"remove\" type=\"button\" aria-label=\"" + sbutton.del + "\" onclick=\"deleteForward(" + i + ")\" />";
	}
}

function deleteForward(x) {
	if (x != 'all') {
		var e = data[x];
		if (!confirm(share.del + " " + e.lanIP + ":" + e.lanPorts + "/" + e.proto + " (" + e.desc + ")?")) return;
	}
	else {
		if (!confirm(upnp.msg2)) return;
	}
	var fupnp = document.getElementById("fupnp");
	fupnp.remove.value = (x == 'all' ? 'all' : e.index);
	fupnp.delete_button.value = sbutton.deleted;
	fupnp.save_button.disabled = true;
	fupnp.delete_button.disabled = true;
	update.stop();
	apply(fupnp);
}

function to_submit(F) {
	F.save_button.value = sbutton.saving;
	apply(F);
}

function to_apply(F) {
	F.apply_button.value = sbutton.applied;
	applytake(F);
}

var update;

addEvent(window, "load", function() {
	stickControl(<% nvg("sticky_footer"); %>);
	setUPnPTable(new Array(<% tf_upnp(); %>));

	update = new StatusUpdate("UPnP.live.asp", <% nvg("refresh_time"); %>);
	update.onUpdate("upnp_forwards", function(u) {
		setUPnPTable(eval("new Array(" + u.upnp_forwards + ")"));
	});
	update.start();
});

addEvent(window, "unload", function() {
	update.stop();
});

	//]]>
	</script>
	</head>

	<body class="gui">
		<div id="wrapper">
			<div id="content">
				<div id="header">
					<div id="logo"><h1><% show_control(); %></h1></div>
					<% do_menu("ForwardSpec.asp","UPnP.asp"); %>
				</div>
				<div id="main">
					<div id="contents">
						<form name="fupnp" id="fupnp" action="apply.cgi" method="post">
							<input type="hidden" name="submit_button" value="UPnP" />
							<input type="hidden" name="action" value="Apply" />
							<input type="hidden" name="change_action" />
							<input type="hidden" name="submit_type" />
							<input type="hidden" name="commit" value="1" />
							<input type="hidden" name="remove" />

							<h2><% tran("upnp.h2"); %></h2>
							<fieldset>
								<legend><% tran("upnp.legend"); %></legend>
								<table class="table" cellspacing="6" id="upnp_table" summary="UPnP table">
									<tbody>
									<tr>
										<th width="10%"><% tran("share.ip"); %></th>
										<th width="7%"><% tran("pforward.from"); %></th>
										<th width="7%"><% tran("pforward.to"); %></th>
										<th width="5%"><% tran("share.proto"); %></th>
										<th width="30%"><% tran("share.descr"); %></th>
										<th width="10%" class="center"><% tran("share.del"); %></th>
									</tr>
									</tbody>
								</table><br />
								<div class="center">
									<script type="text/javascript">
									//<![CDATA[
									document.write("<input class=\"button\" type=\"button\" name=\"delete_button\" value=\"" + sbutton.delall + "\" onclick=\"deleteForward('all');\" />");
									//]]>
									</script>
								</div>
							</fieldset><br />
							<fieldset>
								<legend><% tran("upnp.legend2"); %></legend>
								<div class="setting">
									<div class="label"><% tran("upnp.serv"); %></div>
									<input class="spaceradio" type="radio" name="upnp_enable" value="1" <% nvc("upnp_enable","1"); %> onclick="setUPnP(this.value)" /><% tran("share.enable"); %>&nbsp;
									<input class="spaceradio" type="radio" name="upnp_enable" value="0" <% nvc("upnp_enable","0"); %> onclick="setUPnP(this.value)" /><% tran("share.disable"); %>
								</div>
							</fieldset><br />
							<div id="footer" class="submitFooter">
								<script type="text/javascript">
								//<![CDATA[
								var autoref = <% nvem("refresh_time","0","sbutton.refres","sbutton.autorefresh"); %>;
								submitFooterButton(1,1,0,autoref);
								//]]>
								</script>
							</div>
						</form>
					</div>
				</div>
				<div id="helpContainer">
					<div id="help">
						<h2><% tran("share.help"); %></h2>
						<dl>
							<dt class="term"><% tran("upnp.legend"); %>:</dt>
							<dd class="definition"><% tran("hupnp.right2"); %></dd>
							<dt class="term"><% tran("upnp.serv"); %>:</dt>
							<dd class="definition"><% tran("hupnp.right4"); %></dd>
						</dl><br />
						<a href="javascript:openHelpWindow<% ifdef("EXTHELP","Ext"); %>('HUPnP.asp')"><% tran("share.more"); %></a>
					</div>
				</div>
				<div id="floatKiller"></div>
				<div id="statusInfo">
				<div class="info"><% tran("share.firmware"); %>:&nbsp;
					<script type="text/javascript">
					//<![CDATA[
					document.write("<a title=\"" + share.about + "\" href=\"javascript:openAboutWindow()\"><% get_firmware_version(); %></a>");
					//]]>
					</script>
				</div>
				<div class="info"><% tran("share.time"); %>:  <span id="uptime"><% get_uptime(); %></span></div>
				<div class="info">WAN<span id="ipinfo"><% show_wanipinfo(); %></span></div>
				</div>
			</div>
		</div>
	</body>
</html>
