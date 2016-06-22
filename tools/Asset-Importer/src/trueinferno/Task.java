package trueinferno;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.io.FileUtils;

public abstract class Task {
	public abstract String getTitle();
	
	public abstract String getDescription();
	
	public abstract boolean getResult();
	
	public abstract String getObservations();
	
	public abstract void execute();
	
	protected void updateAttributeInMap(File mapFile, String entityName, String attributeName, String newValue) throws IOException, TrueInfernoException {
		String oldMapContent = FileUtils.readFileToString(mapFile, (Charset)null);
		Pattern p = Pattern.compile("(" + entityName + "\\s*=\\s*\\{.*?)(" + attributeName + "\\s*=\\s*\".*?\")(.*?\\},)", Pattern.MULTILINE | Pattern.DOTALL);
		Matcher m = p.matcher(oldMapContent);
		
		if(!m.find())
			throw new TrueInfernoException("No definition found in map file '" + mapFile + "' for type '" + entityName + "'");

		if(m.find())
			throw new TrueInfernoException("Multiple definitions found in map file '" + mapFile + "'  for type '" + entityName + "'");
		
		String newMapContent = m.replaceFirst("$1" + attributeName + "= \"" + newValue + "\"$3");
		FileUtils.writeStringToFile(mapFile, newMapContent, (Charset)null);
	}
}
