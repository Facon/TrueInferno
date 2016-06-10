package trueinferno;

public interface Task {
	public String getTitle();
	
	public String getDescription();
	
	public boolean getResult();
	
	public String getObservations();
	
	public void execute();
}
