package trueinferno;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;

public class Util {
	public static String getTimeStamp(){
		return new SimpleDateFormat("yyyyMMddHHmmss").format(new Date());
	}
	
	public static void backupFile(File file) throws IOException{
		File dir = new File(FilenameUtils.getFullPath(file.getAbsolutePath()));
		String extension = FilenameUtils.getExtension(file.getName());
		String baseName = FilenameUtils.getBaseName(file.getName());
		
		File backupFile = FileUtils.getFile(dir, baseName + "_" + Util.getTimeStamp() + "." + extension);
		FileUtils.moveFile(file, backupFile);
	}

	public static void copyWithBackup(File originFile, File destinationFile) throws IOException {
		// Si existía previamente el fichero
		if(destinationFile.exists()){
			// Y es idéntico al nuevo, no hacemos nada
			if(FileUtils.contentEquals(originFile, destinationFile)){
				return;
			}
			
			// Si es distinto al nuevo, hacemos backup primero
			else{
				Util.backupFile(destinationFile);
			}
		}
		
		FileUtils.copyFile(originFile, destinationFile);
	}

	public static void copyToDirWithBackup(File originFile, File destinationDir) throws IOException {
		File destinationFile = FileUtils.getFile(destinationDir, originFile.getName());
		copyWithBackup(originFile, destinationFile);
	}
}
