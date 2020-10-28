var child_process = require('child_process');
var fs = require('fs');
var os = require('os');

function getIPAdress() {
    var interfaces = os.networkInterfaces();
    for(var devName in interfaces){
        var iface = interfaces[devName];
        for(var i=0;i<iface.length;i++){
            var alias = iface[i];
            if(alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal){
                return alias.address;
            }
        }
    }
}
function copy(str) {
	var command;
	switch(process.platform) {
		case "darwin":
			command = 'echo "'+str+'"|pbcopy';
			break;
		case "win32":
			command = 'mshta vbscript:clipboarddata.setdata("text","'+str+'")(close)';
			break;
		default:
			throw new Error("Unknown platform: '" + process.platform);
	}
	child_process.exec(
		command,
		function(err, stdout, stderr) {
			if (err) {
				throw new Error("copy error");
			}
		}
	);
}
function paste(callback) {
	var command;
	switch(process.platform) {
		case "darwin":
			child_process.exec(
				'pbpaste',
				function(err, stdout, stderr) {
					if (err) {
						throw new Error("copy error");
					}
					callback(stdout);
				}
			);
			break;
		case "win32":
			child_process.exec('uname',
				function(err, stdout, stderr) {
					if (!err) {
						return child_process.exec('cat /dev/clipboard',
							function(err, stdout, stderr) {
								if (err) {
									console.log(err);
									throw new Error("copy error");
								}
								callback(stdout);
							}
						);
					}
					var paste_vbs='Set objHTML = CreateObject("htmlfile")\n'
					+'text = objHTML.ParentWindow.ClipboardData.GetData("Text")\n'
					+'Wscript.Echo text';
					fs.writeFileSync('paste.vbs', paste_vbs);
					child_process.exec('cscript /nologo paste.vbs',
						function(err, stdout, stderr) {
							if (err) {
								console.log(err);
								throw new Error("copy error");
							}
							callback(stdout);
							fs.unlinkSync('paste.vbs');
						}
					);
				}
			);
			break;
		default:
			throw new Error("Unknown platform: '" + process.platform);
	}
}

module.exports = {
    copy,
    paste,
    getIPAdress,
}
