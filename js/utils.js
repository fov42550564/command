var child_process = require('child_process');
var fs = require('fs');
var os = require('os');

function checkIdNo (idNo) {
    if (!idNo) {
        return console.log('empty');
    }
    const number = idNo.split('');
    const W = [ 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 ];// 加权因子
    const ValideCode = [ 1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2 ];// 身份证验证位值.10代表X
    if (!number || number.length !== 18) {
        return console.log('error idNo');
    }
    if (number[17] === 'x' || number[17] === 'X') {
        number[17] = 10;
    }
    let sum = 0;
    for (let i = 0; i < 17; i++) {
        sum += W[i] * number[i];
    }
    if (number[17] != ValideCode[sum % 11]) {
        return console.log(`[checkIdNo]: ${idNo} last number should be ${ValideCode[sum % 11]}`);
    }
    return console.log('success');
}
function getIPAdress () {
    var ip;
    var interfaces = os.networkInterfaces();
    for(var devName in interfaces){
        var iface = interfaces[devName];
        for(var i=0;i<iface.length;i++){
            var alias = iface[i];
            if(alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal){
                ip = alias.address;
            }
        }
    }
    return ip;
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
    checkIdNo,
}
