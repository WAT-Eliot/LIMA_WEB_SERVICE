﻿//------------------------------------------------------------------------------
// <auto-generated>
//     Ce code a été généré par un outil.
//     Version du runtime :4.0.30319.42000
//
//     Les modifications apportées à ce fichier peuvent provoquer un comportement incorrect et seront perdues si
//     le code est régénéré.
// </auto-generated>
//------------------------------------------------------------------------------

namespace usrmgrDotNetProject.CollectShuttleToMx {
    using System.Runtime.Serialization;
    using System;
    
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="PostTapingCollectShuttleTapingOutputConveyorRequest", Namespace="http://schemas.datacontract.org/2004/07/Mx.Broker.Std.Request.Interface.PostTapin" +
        "gCollectShuttle01")]
    [System.SerializableAttribute()]
    public partial class PostTapingCollectShuttleTapingOutputConveyorRequest : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int TapingOutputConveyorNumberField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int TapingOutputConveyorNumber {
            get {
                return this.TapingOutputConveyorNumberField;
            }
            set {
                if ((this.TapingOutputConveyorNumberField.Equals(value) != true)) {
                    this.TapingOutputConveyorNumberField = value;
                    this.RaisePropertyChanged("TapingOutputConveyorNumber");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="PostTapingCollectShuttleMissionStatusRequest", Namespace="http://schemas.datacontract.org/2004/07/Mx.Broker.Std.Request.Interface.PostTapin" +
        "gCollectShuttle01")]
    [System.SerializableAttribute()]
    public partial class PostTapingCollectShuttleMissionStatusRequest : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int MissionNumberField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleMissionStatusType MissionStatusField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int MissionNumber {
            get {
                return this.MissionNumberField;
            }
            set {
                if ((this.MissionNumberField.Equals(value) != true)) {
                    this.MissionNumberField = value;
                    this.RaisePropertyChanged("MissionNumber");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleMissionStatusType MissionStatus {
            get {
                return this.MissionStatusField;
            }
            set {
                if ((this.MissionStatusField.Equals(value) != true)) {
                    this.MissionStatusField = value;
                    this.RaisePropertyChanged("MissionStatus");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="PostTapingCollectShuttleMissionStatusType", Namespace="http://schemas.datacontract.org/2004/07/Mx.Broker.Std.Request.Interface.PostTapin" +
        "gCollectShuttle01")]
    public enum PostTapingCollectShuttleMissionStatusType : int {
        
        [System.Runtime.Serialization.EnumMemberAttribute()]
        Ok = 1,
        
        [System.Runtime.Serialization.EnumMemberAttribute()]
        Nok = 2,
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="PostTapingCollectShuttleReportRunningModeRequest", Namespace="http://schemas.datacontract.org/2004/07/Mx.Broker.Std.Request.Interface.PostTapin" +
        "gCollectShuttle01")]
    [System.SerializableAttribute()]
    public partial class PostTapingCollectShuttleReportRunningModeRequest : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private System.Nullable<int> DefaultCodeField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private string DefaultLabelField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int EquipmentCodeField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleRunningModeType RunningModeField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private System.DateTime RunningModeDateTimeField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public System.Nullable<int> DefaultCode {
            get {
                return this.DefaultCodeField;
            }
            set {
                if ((this.DefaultCodeField.Equals(value) != true)) {
                    this.DefaultCodeField = value;
                    this.RaisePropertyChanged("DefaultCode");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string DefaultLabel {
            get {
                return this.DefaultLabelField;
            }
            set {
                if ((object.ReferenceEquals(this.DefaultLabelField, value) != true)) {
                    this.DefaultLabelField = value;
                    this.RaisePropertyChanged("DefaultLabel");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int EquipmentCode {
            get {
                return this.EquipmentCodeField;
            }
            set {
                if ((this.EquipmentCodeField.Equals(value) != true)) {
                    this.EquipmentCodeField = value;
                    this.RaisePropertyChanged("EquipmentCode");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleRunningModeType RunningMode {
            get {
                return this.RunningModeField;
            }
            set {
                if ((this.RunningModeField.Equals(value) != true)) {
                    this.RunningModeField = value;
                    this.RaisePropertyChanged("RunningMode");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public System.DateTime RunningModeDateTime {
            get {
                return this.RunningModeDateTimeField;
            }
            set {
                if ((this.RunningModeDateTimeField.Equals(value) != true)) {
                    this.RunningModeDateTimeField = value;
                    this.RaisePropertyChanged("RunningModeDateTime");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="PostTapingCollectShuttleRunningModeType", Namespace="http://schemas.datacontract.org/2004/07/Mx.Broker.Std.Request.Interface.PostTapin" +
        "gCollectShuttle01")]
    public enum PostTapingCollectShuttleRunningModeType : int {
        
        [System.Runtime.Serialization.EnumMemberAttribute()]
        Manual = 1,
        
        [System.Runtime.Serialization.EnumMemberAttribute()]
        Rest = 2,
        
        [System.Runtime.Serialization.EnumMemberAttribute()]
        MissionInProgress = 3,
        
        [System.Runtime.Serialization.EnumMemberAttribute()]
        Default = 4,
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    [System.ServiceModel.ServiceContractAttribute(ConfigurationName="CollectShuttleToMx.IInterfacesPostTapingCollectShuttleToMx01")]
    public interface IInterfacesPostTapingCollectShuttleToMx01 {
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/TapingOutputConveyor" +
            "Number", ReplyAction="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/TapingOutputConveyor" +
            "NumberResponse")]
        void TapingOutputConveyorNumber(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleTapingOutputConveyorRequest request);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/TapingOutputConveyor" +
            "Number", ReplyAction="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/TapingOutputConveyor" +
            "NumberResponse")]
        System.Threading.Tasks.Task TapingOutputConveyorNumberAsync(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleTapingOutputConveyorRequest request);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/MissionStatus", ReplyAction="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/MissionStatusRespons" +
            "e")]
        void MissionStatus(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleMissionStatusRequest request);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/MissionStatus", ReplyAction="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/MissionStatusRespons" +
            "e")]
        System.Threading.Tasks.Task MissionStatusAsync(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleMissionStatusRequest request);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/ReportRunningMode", ReplyAction="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/ReportRunningModeRes" +
            "ponse")]
        void ReportRunningMode(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleReportRunningModeRequest request);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/ReportRunningMode", ReplyAction="http://tempuri.org/IInterfacesPostTapingCollectShuttleToMx01/ReportRunningModeRes" +
            "ponse")]
        System.Threading.Tasks.Task ReportRunningModeAsync(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleReportRunningModeRequest request);
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public interface IInterfacesPostTapingCollectShuttleToMx01Channel : usrmgrDotNetProject.CollectShuttleToMx.IInterfacesPostTapingCollectShuttleToMx01, System.ServiceModel.IClientChannel {
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public partial class InterfacesPostTapingCollectShuttleToMx01Client : System.ServiceModel.ClientBase<usrmgrDotNetProject.CollectShuttleToMx.IInterfacesPostTapingCollectShuttleToMx01>, usrmgrDotNetProject.CollectShuttleToMx.IInterfacesPostTapingCollectShuttleToMx01 {
        
        public InterfacesPostTapingCollectShuttleToMx01Client() {
        }
        
        public InterfacesPostTapingCollectShuttleToMx01Client(string endpointConfigurationName) : 
                base(endpointConfigurationName) {
        }
        
        public InterfacesPostTapingCollectShuttleToMx01Client(string endpointConfigurationName, string remoteAddress) : 
                base(endpointConfigurationName, remoteAddress) {
        }
        
        public InterfacesPostTapingCollectShuttleToMx01Client(string endpointConfigurationName, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(endpointConfigurationName, remoteAddress) {
        }
        
        public InterfacesPostTapingCollectShuttleToMx01Client(System.ServiceModel.Channels.Binding binding, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(binding, remoteAddress) {
        }
        
        public void TapingOutputConveyorNumber(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleTapingOutputConveyorRequest request) {
            base.Channel.TapingOutputConveyorNumber(request);
        }
        
        public System.Threading.Tasks.Task TapingOutputConveyorNumberAsync(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleTapingOutputConveyorRequest request) {
            return base.Channel.TapingOutputConveyorNumberAsync(request);
        }
        
        public void MissionStatus(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleMissionStatusRequest request) {
            base.Channel.MissionStatus(request);
        }
        
        public System.Threading.Tasks.Task MissionStatusAsync(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleMissionStatusRequest request) {
            return base.Channel.MissionStatusAsync(request);
        }
        
        public void ReportRunningMode(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleReportRunningModeRequest request) {
            base.Channel.ReportRunningMode(request);
        }
        
        public System.Threading.Tasks.Task ReportRunningModeAsync(usrmgrDotNetProject.CollectShuttleToMx.PostTapingCollectShuttleReportRunningModeRequest request) {
            return base.Channel.ReportRunningModeAsync(request);
        }
    }
}
