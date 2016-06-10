package trueinferno;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.FileFilterUtils;
import org.apache.commons.io.filefilter.IOFileFilter;
import org.apache.commons.io.filefilter.TrueFileFilter;

public class ImportMaterialTask implements Task {

	public enum MaterialTaskOption {
		IMPORT_CREATE,
		SKIP
	}
	
	/** Extensiones válidas de un fichero de material */
	private static final String[] MATERIAL_EXTENSIONS = {"material"};
	
	/** Extensiones válidas de un fichero de textura */
	private static final String[] TEXTURE_EXTENSIONS = {"jpg","png"};
	
	private String realName;
	private String givenName;
	private boolean result;
	private String observations;

	private File inputModelDir;
	private File outputMaterialDir;
	private File outputTextureDir;
	private File mapFile;

	// Nombre correcto del fichero de material
	private File materialFile;

	// Nombre correcto del fichero de textura con el difuso/albedo
	private File albedoTextureFile;

	// Filtro de ficheros que acepta ficheros de textura de Albedo (difuso)
	private IOFileFilter isAlbedoFileFilter;

	private MaterialTaskOption materialTaskOption;
	
	public ImportMaterialTask(String realName, String givenName, File inputModelDir, File outputMaterialDir, File outputTextureDir, File mapFile, MaterialTaskOption materialTaskOption) {
		this.realName = realName;
		this.givenName = givenName;
		this.inputModelDir = inputModelDir;
		this.outputMaterialDir = outputMaterialDir;
		this.outputTextureDir = outputTextureDir;
		this.mapFile = mapFile;
		this.materialTaskOption = materialTaskOption;
		
		materialFile = FileUtils.getFile(outputMaterialDir, realName+".material");
		albedoTextureFile = FileUtils.getFile(outputTextureDir, realName + "_" + "Albedo.png");
		 
		isAlbedoFileFilter = 
				FileFilterUtils.asFileFilter(
						new FileFilter(){
							@Override
							public boolean accept(File pathname) {
								String extension = FilenameUtils.getExtension(pathname.getName()).toLowerCase();
								String name = pathname.getName().toLowerCase();

								return Arrays.asList(TEXTURE_EXTENSIONS).contains(extension) && name.matches(".*(albedo|diff?us).*"); 
							}
						}
						);				
	}

	@Override
	public String getTitle() {
		return "ImportMaterial";
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
			importMaterial();
			result = true;
			observations = "";
		}
		catch(Exception e){
			Logger.getGlobal().log(Level.INFO, "Error in " + getTitle() + " - " + getDescription(), e);
			result = false;
			observations = e.getMessage();
		}
	}

	private void importMaterial() throws TrueInfernoException, IOException {
		// Buscamos el fichero con el material
		File originMaterialFile = null;
		try{
			originMaterialFile = findMaterialFile();
			// Copiamos el material a su sitio correcto
			Util.copyWithBackup(originMaterialFile, materialFile);
		}
		
		// Si no se encontró el asset del material lo creamos directamente
		catch (AssetFileNotFoundException e) {
			createMaterialFile(materialFile);
		}
	}

	private void createMaterialFile(File destinationMaterialFile) throws IOException, TrueInfernoException {
		// Localizamos el fichero de origen con la textura de Albedo 
		File originAlbedoFile = findAlbedoTextureFile();
		
		// Lo copiamos a su sitio
		Util.copyWithBackup(originAlbedoFile, albedoTextureFile);
		
		// Creamos un nuevo fichero de material que haga uso de la textura de Albedo localizada
		File newMaterialFile = File.createTempFile(realName, Util.getTimeStamp());

		ArrayList<String> lines = new ArrayList<String>();
		lines.add("material "+realName);
		lines.add("{");
		lines.add("	technique");
		lines.add("	{");
		lines.add("		pass");
		lines.add("		{");
		lines.add("	");
		lines.add("			texture_unit");
		lines.add("			{");
		lines.add("				texture building/"+albedoTextureFile.getName());
		lines.add("			}");
		lines.add("		");
		lines.add("		}");
		lines.add("	}");
		lines.add("}");
		
		FileUtils.writeLines(newMaterialFile, lines);
		
		// Escribimos el fichero de material con backup si es necesario
		Util.copyWithBackup(newMaterialFile, destinationMaterialFile);
	}

	private File findMaterialFile() throws TrueInfernoException {
		File dir = FileUtils.getFile(inputModelDir, givenName);
		
		Collection<File> files = FileUtils.listFiles(dir, MATERIAL_EXTENSIONS, true);
		if(files.size() > 1)
			throw new TrueInfernoException("Multiple material files found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"': "+files);
		else if (files.isEmpty())
			throw new AssetFileNotFoundException("No material file found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"'");
		
		return files.iterator().next();
	}
	
	private File findAlbedoTextureFile() throws TrueInfernoException {
		File dir = FileUtils.getFile(inputModelDir, givenName);
		
		Collection<File> files = FileUtils.listFiles(dir, isAlbedoFileFilter, TrueFileFilter.TRUE);
		if(files.size() > 1)
			throw new TrueInfernoException("Multiple texture files found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"': "+files);
		else if (files.isEmpty())
			throw new AssetFileNotFoundException("No texture file found for model name '"+realName+"' using alias '"+givenName+"' in directory '"+dir+"'");
		
		return files.iterator().next();
	}
	
}
