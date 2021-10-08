import * as fs from 'fs';
import * as path from 'path';

function splitSrcTxt() {
    let dir: string = `E:\\Work\\cpp\\JamellaD2E\\docs`;
    let srcFilePath: string = `${dir}\\pdf.txt`;
    
    let src: string = fs.readFileSync(srcFilePath, "UTF-8");
    
    let pattern: RegExp = /\n\d+\n(.+)\n/g;
    let matches: IterableIterator<RegExpMatchArray> = src.matchAll(pattern);
    let match: IteratorResult<RegExpMatchArray> = matches.next();

    let pageCount: number = 0;
    let pages: Array<{
        fileName: string;
        content: string;
    }> = [];

    while (match.done == false) {
        console.log(`${match.value[1]}`);

        pages[pageCount] = {
            fileName: match.value[1],
            content: ""
        };

        pageCount++;
        match = matches.next();
    }

    let pattern2: RegExp = /\n\d+\n.+\n/g;
    let pageContents: Array<string> = src.split(pattern2);

    pageContents.shift();

    for (let i = 0; i < pageContents.length; i++) {
        pages[i].content = pageContents[i];
    }

    let result: {
        [fileName: string]: string;
    } = {};

    for (let i = 0; i < pages.length; i++) {
        if (result[pages[i].fileName] == undefined) {
            result[pages[i].fileName] = pages[i].content;
        }
        else {
            result[pages[i].fileName] += `\r\n` + pages[i].content;
        }
    }

    for (var fileName in result) {
        fs.writeFileSync(`${dir}\\JamellaD2E\\${fileName}`, result[fileName]);
    }
}

function copyMissingFiles() {
    let s: string = fs.readFileSync("test.txt", "UTF-8");
    let a: Array<string> = s.split("\r\n");
    let dir: string = `E:\\Work\\cpp\\JamellaD2E\\vs2019\\JamellaD2E`;
    for (let i = 0; i < a.length; i++) {
        let filePath: string = `${dir}\\${a[i]}`;
        if (fs.existsSync(filePath)) {

        }
        else {
            fs.copyFileSync("000.bmp", filePath)
        }
    }
}

function copyMissingFiles2() {
    let srcDir: string = `res/D2S`;
    let outDir: string = `res/Result`;

    let srcRcPath: string = `${srcDir}/1.rc`;

    let jamellaSrcDir: string = `E:/Work/cpp/JamellaD2E/vs2019/JamellaD2E`;
    let jamellaResID: string = fs.readFileSync(`${jamellaSrcDir}/ResourceIDs.h`, "UTF-8");
    let jamellaRc: string = fs.readFileSync(`${jamellaSrcDir}/JamellaD2E.rc`, "UTF-8");

    let srcRc: string = fs.readFileSync(srcRcPath, "UTF-8");

    let pattern: RegExp = /(\d+)\s\S+\s"([^"]+)"/g;
    let matches: IterableIterator<RegExpMatchArray> = srcRc.matchAll(pattern);
    let match: IteratorResult<RegExpMatchArray> = matches.next();

    let resources: Array<{
        id: string;
        fileName: string;
        jamellaDefinedId: string;
        jamellaPath: string;
    }> = [];

    while (match.done == false) {
        let id: string = match.value[1];
        let fileName: string = match.value[2];

        let patternId: RegExp = new RegExp(`#define\\s(\\S+)\\s${id}`);
        let ea: RegExpExecArray = patternId.exec(jamellaResID);

        let jamellaDefinedId: string = ea[1];

        let patternPath: RegExp = new RegExp(`${jamellaDefinedId}\\s\\S+\\s\\S+\\s"([^"]+)"`);
        ea = patternPath.exec(jamellaRc);
        
        resources.push({
            id: id,
            fileName: fileName,
            jamellaDefinedId: jamellaDefinedId,
            jamellaPath: ea[1]
        });

        match = matches.next();
    }

    for (let i = 0; i < resources.length; i++) {
        let srcPath: string = `${srcDir}/${resources[i].fileName}`;
        let destPath: string = path.resolve(`${outDir}/${resources[i].jamellaPath}`);
        let destPathDir: string = path.dirname(destPath);
        
        fs.mkdirSync(destPathDir, {
            recursive: true
        });

        fs.copyFileSync(srcPath, destPath);
    }
}

(function main(){
    copyMissingFiles();
})()
