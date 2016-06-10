package trueinferno;

import java.io.File;
import java.util.Collection;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.commons.io.FileUtils;

public class ImportMeshTask implements Task {
	public enum MeshTaskOption {
		IMPORT,
		SKIP
	}
	
	/** Extensiones válidas de un fichero de malla */
	private static final String[] MESH_EXTENSIONS = {"mesh"};
	
	private String realName;
	private String givenName;
	private boolean result;
	private String observations;

	private File inputModelDir;
	private File outputModelDir;
	private File mapFile;

	private MeshTaskOption meshTaskOption;

	public ImportMeshTask(String realName, String givenName, File inputModelDir, File outputModelDir, File mapFile, MeshTaskOption meshTaskOption) {
		this.realName = realName;
		this.givenName = givenName;
		this.inputModelDir = inputModelDir;
		this.outputModelDir = outputModelDir;
		this.mapFile = mapFile;
		this.meshTaskOption = meshTaskOption;
	}

	@Override
	public String getTitle() {
		return "ImportMesh";
	}

	@Override
	public String getDescription() {
		return realName;
	}

	@Override
	public boolean getResult() {
		return result;
	}

	@Override
	public String getObservations() {
		return observations;
	}

	@Override
	public void execute() {
		try{
			importMesh();
			result = true;
			observations = "";
		}
		catch(Exception e){
			Logger.getGlobal().log(Level.INFO, "Error in " + getTitle() + " - " + getDescription(), e);
			result = false;
			observations = e.getMessage();
		}
	}

	private void importMesh() throws TrueInfernoException {
		// Buscamos el fichero con la malla
		File meshFile = findMeshFile();
		
		// TODO
		throw new TrueInfernoException("TODO");
	}

	private File findMeshFile() throws TrueInfernoException {
		File dir = FileUtils.getFile(inputModelDir, givenName);
		
		Collection<File> files = FileUtils.listFiles(dir, MESH_EXTENSIONS, true);
		if(files.size() > 1)
			throw new TrueInfernoException("Multiple mesh files found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"': "+files);
		else if (files.isEmpty())
			throw new AssetFileNotFoundException("No mesh file found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"'");
		
		return files.iterator().next();
	}
	
}
